#include "../includes/comm_Mem.h"

void solicitar_tabla_segmentos(){
	serializar_solicitud_tabla(memoria_connection);

	t_paquete *paquete = malloc(sizeof(t_paquete));
	paquete->buffer = malloc(sizeof(t_buffer));

	deserializar_header(paquete, memoria_connection);
	switch (paquete->codigo_operacion){
		case 1:
			deserializar_tabla_segmentos(lista_tabla_segmentos,paquete->buffer,paquete->lineas);
			log_info(logger,"Tabla de segmentos actualizada");
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
				log_error(logger,"Fallo de serializacion  de respuesta memoria");
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
			log_info(logger,"success create segment");
			break;
		case OUT_OF_MEMORY:
			log_info(logger,"out of memory");
			break;
		case COMPACTION_NEEDED:
			serializar_solicitud_compactacion(memoria_connection);
			create_segment(contexto,pcb);
			log_info(logger,"compaction needed");
			break;
		default:
			break;
	}
}

void delete_segment(t_contexto* contexto, pcb_t* pcb){
	t_instruc_mem* instruccion_delete = inicializar_instruc_mem();
	copiar_instruccion_mem(instruccion_delete,contexto);
	serializar_instruccion_memoria(memoria_connection, instruccion_delete);
}
