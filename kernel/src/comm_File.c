#include "../includes/comm_File.h"

void manejar_archivo(t_contexto* contexto, pcb_t* pcb){
	t_instruc_file* instruccion = inicializar_instruc_file();
	copiar_instruccion_file(instruccion,contexto);
	serializar_instruccion_file(file_system_connection, instruccion);

	t_resp_file respuesta = esperar_respuesta_file();

	switch(respuesta){
		case F_OPEN_SUCCESS:
			log_info(logger, "PID: %d - Abrir Archivo: %s", pcb->pid, contexto->param1);
			log_info(logger,"F_OPEN success");
			break;
		case F_CLOSE_SUCCESS:
			log_info(logger, "PID: %d - Cerrar Archivo: %s", pcb->pid, contexto->param1);
			log_info(logger,"F_CLOSE success");
			break;
		case F_TRUNCATE_SUCCESS:
			log_info(logger,"F_TRUNCATE success");
			break;
		case F_SEEK_SUCCESS:
			log_info(logger,"F_SEEK success");
			log_info(logger, "PID: %d - Actualizar puntero Archivo: %s - Puntero: %s", pcb->pid, contexto->param1, contexto->param2);
			break;
		case FILE_DOESNT_EXISTS:
			log_info(logger,"F_CREATE required");
			contexto->estado = F_CREATE;
			manejar_archivo(contexto,pcb);
			break;
		case FILE_ALREADY_EXISTS:
			log_info(logger,"Archivo %s ya previamente abierto",contexto->param1);
			break;
		default:
			break;
	}
}

void editar_archivo(t_contexto* contexto, pcb_t* pcb){
	t_instruc_file* instruccion = inicializar_instruc_file();
	copiar_instruccion_file(instruccion,contexto);
	serializar_instruccion_file(file_system_connection, instruccion);

	t_resp_file respuesta = esperar_respuesta_file();

	switch(respuesta){
		case F_WRITE_SUCCESS:
			log_info(logger,"F_WRITE success");
			break;
		case F_READ_SUCCESS:
			log_info(logger,"F_READ success");
			break;
		default:
			break;
	}
}

t_resp_file esperar_respuesta_file(){

		t_resp_file respuesta = F_ERROR;
		t_paquete *paquete = malloc(sizeof(t_paquete));
		paquete->buffer = malloc(sizeof(t_buffer));

		deserializar_header(paquete, file_system_connection);
		switch (paquete->codigo_operacion){
			case 1:
				t_resp_file resp = deserializar_respuesta_file(paquete->buffer) ;
				respuesta = resp;
				break;
			default:
				log_error(logger,"Fallo de serializacion de respuesta file");
				break;
		}

		free(paquete->buffer->stream);
		free(paquete->buffer);
		free(paquete);

		return respuesta;
}
