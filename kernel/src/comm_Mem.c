#include "../includes/comm_Mem.h"

void solicitar_tabla_segmentos(){
	serializar_solicitud_tabla(memoria_connection);

	t_paquete *paquete = malloc(sizeof(t_paquete));
	paquete->buffer = malloc(sizeof(t_buffer));

	deserializar_header(paquete, memoria_connection);
	switch (paquete->codigo_operacion){
		case 1:
			deserializar_tabla_segmentos(lista_tabla_segmentos,paquete->buffer,paquete->lineas);
			//log_info(logger,"Tabla de segmentos ACTUALIZADA");
			//imprimir_tabla_segmentos();
			break;
		default:
			log_error(logger,"Fallo la lectura de la tabla de segmentos");
			break;
	}

	free(paquete->buffer->stream);
	free(paquete->buffer);
	free(paquete);
}

t_resp_mem esperar_respuesta_memoria(){

		t_resp_mem respuesta = ERROR;
		t_paquete *paquete = malloc(sizeof(t_paquete));
		paquete->buffer = malloc(sizeof(t_buffer));

		deserializar_header(paquete, memoria_connection);
		switch (paquete->codigo_operacion){
			case 1:
				t_resp_mem resp = deserializar_respuesta_memoria(paquete->buffer) ;
				respuesta = resp;
				break;
			default:
				log_error(logger,"Fallo en comunicacion con MEMORIA");
				break;
		}

		free(paquete->buffer->stream);
		free(paquete->buffer);
		free(paquete);

		return respuesta;
}

void create_segment(t_contexto* contexto, pcb_t* pcb){
	t_instruc_mem* instruccion = inicializar_instruc_mem();
	copiar_instruccion_mem(instruccion,contexto);
	serializar_instruccion_memoria(memoria_connection, instruccion);
    t_resp_mem respuesta = esperar_respuesta_memoria();

	switch(respuesta){
		case SUCCESS_CREATE_SEGMENT:
			log_info(logger,"PID: %d - Crear Segmento - Id: %s - Tamanio: %s", contexto->pid, contexto->param1, contexto->param2);
			solicitar_tabla_segmentos();
			enviar_contexto(pcb);
			break;
		case OUT_OF_MEMORY:
			log_info(logger,"ERROR OUT OF MEMORY");
			log_info(logger, "PID: %d - Estado Anterior: PCB_EXEC - Estado Actual: PCB_EXIT", pcb->pid);
			list_push(pcb_exit_list, pcb);
			sem_post(&sem_estado_exit);
			break;
		case COMPACTION_NEEDED:
			log_info(logger,"Solicitud de COMPACTACION recibida, esperando Fin de Operaciones de FS");
			sem_wait(&sem_compactacion);
			serializar_solicitud_compactacion(memoria_connection);
			t_resp_mem resp_comp = esperar_respuesta_memoria();
			sem_post(&sem_compactacion);
			if(resp_comp == COMPACTATION_SUCCESS) log_info(logger,"COMPACTACION Finalizada");
			else {
				log_info(logger,"ERROR COMPACTACION");
				log_info(logger, "PID: %d - Estado Anterior: PCB_EXEC - Estado Actual: PCB_EXIT", pcb->pid);
				list_push(pcb_exit_list, pcb);
				sem_post(&sem_estado_exit);
				break;
			}
			solicitar_tabla_segmentos();
			create_segment(contexto,pcb);
			break;
		default:
			break;
	}
}

void delete_segment(t_contexto* contexto, pcb_t* pcb){
	t_instruc_mem* instruccion_delete = inicializar_instruc_mem();
	copiar_instruccion_mem(instruccion_delete,contexto);
	serializar_instruccion_memoria(memoria_connection, instruccion_delete);
	log_info(logger,"PID: %d - Eliminar Segmento - Id: %s", contexto->pid, contexto->param1);
}
