#include "../includes/comm_ThreadKernel.h"

void comm_threadKernel(int kernel_connection){
	while(exit_status == 0){
		t_paquete *paquete = malloc(sizeof(t_paquete));
		paquete->buffer = malloc(sizeof(t_buffer));
		deserializar_header(paquete, kernel_connection);

		switch (paquete->codigo_operacion){
			case 1:
				t_instruc_file *nueva_instruccion = inicializar_instruc_file();
				deserializar_instruccion_file(nueva_instruccion, paquete->buffer, paquete->lineas);
				uint32_t pid = nueva_instruccion->pid;
				t_resp_file estado_file = F_ERROR;
				switch (nueva_instruccion->estado){
					case F_OPEN:
						FILE *fcb_to_open;
						char *open_file_name = strcat(path_fcb_folder,nueva_instruccion->param1);
						fcb_to_open = fopen(open_file_name,"r");
						if(fcb_to_open != NULL) {
							fcb_t *openedFcb = iniciar_fcb(open_file_name);
							openedFcb->nombre_archivo=config_get_string_value(fcb_to_open, "NOMBRE_ARCHIVO");
							log_info(logger, "%s", openedFcb->nombre_archivo);
							openedFcb->tamanio_archivo=config_get_string_value(fcb_to_open, "TAMANIO_ARCHIVO");
							log_info(logger, "%s", openedFcb->tamanio_archivo);
							openedFcb->puntero_directo = config_get_int_value(fcb_to_open, "PUNTERO_DIRECTO");
							log_info(logger, "%d", openedFcb->puntero_directo);
							openedFcb->puntero_indirecto = config_get_int_value(fcb_to_open, "PUNTERO_INDIRECTO");
							log_info(logger, "%d", openedFcb->puntero_indirecto);
							log_info(logger, "FCB Creado");
							list_add(fcb_list,openedFcb);
						}
						else {
							log_info(logger,"El archivo %s solicitado no existe", nueva_instruccion->param1);
						}
						estado_file = F_OPEN_SUCCESS;
						//estado_file = FILE_DOESNT_EXISTS; si el FCB no existe
						log_info(logger,"PID: %d solicito F_OPEN para el archivo %s",pid, nueva_instruccion->param1);
						serializar_respuesta_file_kernel(kernel_connection, estado_file);
						break;
					case F_CREATE:
						FILE *fcb_to_create;
						// Crear archivo en path especifico
						char *create_file_name = strcat(path_fcb_folder,nueva_instruccion->param1);
						fcb_to_create = fopen(create_file_name,"w");
						fclose(fcb_to_create);
						//Agregarle los valores correspondientes
						fcb_to_create = fopen(create_file_name,"a+");
						fprintf(fcb_to_create,"NOMBRE_ARCHIVO:%s\n",nueva_instruccion->param1);
						fprintf(fcb_to_create,"TAMANIO_ARCHIVO:0");
						fprintf(fcb_to_create,"PUNTERO_DIRECTO:0");
						fprintf(fcb_to_create,"PUNTERO_INDIRECTO:0");
						fclose(fcb_to_create);
						//Crear el tcb y guardarlo en la lista
						fcb_t *newFcb = iniciar_fcb(create_file_name);
						newFcb->nombre_archivo=config_get_string_value(fcb_to_create, "NOMBRE_ARCHIVO");
						log_info(logger, "%s", newFcb->nombre_archivo);
						newFcb->tamanio_archivo=config_get_string_value(fcb_to_create, "TAMANIO_ARCHIVO");
						log_info(logger, "%s", newFcb->tamanio_archivo);
						newFcb->puntero_directo = config_get_int_value(fcb_to_create, "PUNTERO_DIRECTO");
						log_info(logger, "%d", newFcb->puntero_directo);
						newFcb->puntero_indirecto = config_get_int_value(fcb_to_create, "PUNTERO_INDIRECTO");
						log_info(logger, "%d", newFcb->puntero_indirecto);
						log_info(logger, "FCB Creado");
						list_add(fcb_list,newFcb);
						estado_file = F_CREATE_SUCCESS;
						log_info(logger,"PID: %d solicito F_CREATE para el archivo %s",pid, nueva_instruccion->param1);
						serializar_respuesta_file_kernel(kernel_connection, estado_file);
						break;
					case F_CLOSE:
						estado_file = F_CLOSE_SUCCESS;
						log_info(logger,"PID: %d solicito F_CLOSE para el archivo %s",pid, nueva_instruccion->param1);
						serializar_respuesta_file_kernel(kernel_connection, estado_file);
						break;
					case F_TRUNCATE:
						estado_file = F_TRUNCATE_SUCCESS;
						log_info(logger,"PID: %d solicito F_TRUNCATE para el archivo %s",pid, nueva_instruccion->param1);
						serializar_respuesta_file_kernel(kernel_connection, estado_file);
						break;
					case F_WRITE:
						realizar_f_write(nueva_instruccion);
						estado_file = F_WRITE_SUCCESS;
						log_info(logger,"PID: %d solicito F_WRITE para el archivo %s",pid, nueva_instruccion->param1);
						serializar_respuesta_file_kernel(kernel_connection, estado_file);
						break;
					case F_READ:
						realizar_f_read(nueva_instruccion);
						estado_file = F_READ_SUCCESS;
						log_info(logger,"PID: %d solicito F_READ para el archivo %s",pid, nueva_instruccion->param1);
						serializar_respuesta_file_kernel(kernel_connection, estado_file);
						break;
					case F_SEEK:
						estado_file = F_SEEK_SUCCESS;
						log_info(logger,"PID: %d solicito F_SEEK para el archivo %s",pid, nueva_instruccion->param1);
						serializar_respuesta_file_kernel(kernel_connection, estado_file);
						break;
					default:
						break;
				}
				free(nueva_instruccion->param1);
				free(nueva_instruccion->param2);
				free(nueva_instruccion->param3);
				free(nueva_instruccion);
				break;
			default:
				exit_status = 1;
				break;
		}
		free(paquete->buffer->stream);
		free(paquete->buffer);
		free(paquete);
	}
}
