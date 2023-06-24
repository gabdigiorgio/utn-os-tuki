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
						bool buscar_archivo(char *nombre_archivo)
						{
							for (int i = 0; i < list_size(fcb_list); i++)
							{
								fcb_t *fcb = (fcb_t*) list_get(fcb_list, i);
								if (fcb->nombre_archivo	== nombre_archivo)
								{
									return true;
								}
							}
							return false;
						}
						log_info(logger,"PID: %d solicito F_OPEN para el archivo %s",pid, nueva_instruccion->param1);
						//Genero el path
						if(buscar_archivo(nueva_instruccion->param1)){
							log_info(logger,"El archivo %s solicitado ya esta abierto", nueva_instruccion->param1);
							estado_file = FILE_ALREADY_EXISTS;
						}
						else {
							char *open_file_name = strcat(path_fcb_folder,nueva_instruccion->param1);
							fcb_t *fcb_to_open = iniciar_fcb(open_file_name);
							if(fcb_to_open != NULL) {
								fcb_t *openedFcb = malloc(sizeof(fcb_t*));
								openedFcb->nombre_archivo=config_get_string_value(fcb_to_open, "NOMBRE_ARCHIVO");
								log_info(logger, "%s", openedFcb->nombre_archivo);
								openedFcb->tamanio_archivo=config_get_int_value(fcb_to_open, "TAMANIO_ARCHIVO");
								log_info(logger, "%d", openedFcb->tamanio_archivo);
								openedFcb->puntero_directo = config_get_int_value(fcb_to_open, "PUNTERO_DIRECTO");
								log_info(logger, "%d", openedFcb->puntero_directo);
								openedFcb->puntero_indirecto = config_get_int_value(fcb_to_open, "PUNTERO_INDIRECTO");
								log_info(logger, "%d", openedFcb->puntero_indirecto);
								log_info(logger, "FCB Creado");
								list_add(fcb_list,openedFcb);
								config_destroy(fcb_to_open);
								estado_file = F_OPEN_SUCCESS;
							}
							else {
								log_info(logger,"El archivo %s solicitado no existe", nueva_instruccion->param1);
								estado_file = FILE_DOESNT_EXISTS;
								}
						}
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
						// Se debe encontrar el FCB el cual coincida el nombre del archivo
						char *file_name = nueva_instruccion->param1;
						log_info(logger,"Se size de la lista es %d",list_size(fcb_list));
						list_remove_element(fcb_list,file_name);
						/*int index_of_fcb;
						for(int i = 0; i<list_size(fcb_list);i++){
							fcb_t *fcb_correcto = malloc(sizeof(fcb_t));
							fcb_correcto = fcb_list[i];
							if(strcmp(file_name,fcb_correcto->nombre_archivo)== 0){

							}

						}*/
						log_info(logger,"Se size de la lista es %d",list_size(fcb_list));
						// Luego se debe quitar de la lista de FCB
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
