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
void esperar_respuesta_memoria(){

		t_paquete *paquete = malloc(sizeof(t_paquete));
		paquete->buffer = malloc(sizeof(t_buffer));

		deserializar_header(paquete, memoria_connection);
		switch (paquete->codigo_operacion){
			case 1:
				t_resp_mem* respuesta = deserializar_respuesta_memoria(paquete->buffer) ;
				switch(*respuesta){
				case SUCCESS_CREATE_SEGMENT:
					log_info(logger,"success create segment");
					break;
				case OUT_OF_MEMORY:
					log_info(logger,"out of memory");
					break;
				case COMPACTION_NEEDED:
					log_info(logger,"compaction needed");
					break;
				}
				break;
			default:
				log_error(logger,"Fallo de serializacion  de respuesta memoria");
				break;
		}

		free(paquete->buffer->stream);
		free(paquete->buffer);
		free(paquete);
}
