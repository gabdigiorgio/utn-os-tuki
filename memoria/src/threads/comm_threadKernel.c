#include "../../includes/comm_threadKernel.h"

void conexion_kernel(int server_connection){

	kernel_connection= esperar_cliente(server_connection);
	log_info(logger,"Se conecto KERNEL");

	while (exit_status==0)
	{
		t_paquete *paquete = malloc(sizeof(t_paquete));
		paquete->buffer = malloc(sizeof(t_buffer));
		deserializar_header(paquete, server_connection);

		switch (paquete->codigo_operacion)
		{
			case 1:
				t_instruc_mem *nueva_instruccion = inicializar_instruc_mem();
				deserializar_instruccion_memoria(nueva_instruccion, paquete->buffer, paquete->lineas);
				uint32_t pid = nueva_instruccion->pid;

				switch (nueva_instruccion->estado)
				{
					case CREATE_SEGMENT:

						int id_segmento = atoi(nueva_instruccion->param1);
						int tamanio_segmento = atoi(nueva_instruccion->param2);

						int estado_memoria; // 0: Se puede crear segmento | 1: Out of memory | 2: Se necesita compactación

						if(tamanio_segmento > tam_memoria_restante()){
							log_error(logger, "Out of Memory");
							estado_memoria = OUT_OF_MEMORY;

						}
						else
						{
							// ___ FIRST ____
							if (strcmp(algoritmo_asignacion, "FIRST") == 0)
								estado_memoria = first(pid, id_segmento, tamanio_segmento);

							// ___ BEST ____
							else if (strcmp(algoritmo_asignacion, "BEST") == 0)
								estado_memoria = best(pid, id_segmento, tamanio_segmento);

							// ___ WORST ____
							else if (strcmp(algoritmo_asignacion, "WORST") == 0)
								estado_memoria = worst(pid, id_segmento, tamanio_segmento);

							if(estado_memoria == COMPACTION_NEEDED ){
								log_info(logger, "Se requiere COMPACTACION");
							}
						}

						//responder a kernel usando el numero devuelto por estado_memoria,
						serializar_respuesta_memoria_kernel(server_connection, estado_memoria);
						break;

						case DELETE_SEGMENT:
							int id_segment = atoi(nueva_instruccion->param1);
							tabla_segmentos_t* tabla_de_proceso = buscar_tabla(pid);
							eliminar_segmento(tabla_de_proceso, lista_de_huecos_libres, (uint32_t) id_segment);
							break;
						case DELETE_TABLE:
							tabla_segmentos_t* tabla_de_proceso_to_delete = buscar_tabla(nueva_instruccion->pid);
							while(list_size(tabla_de_proceso_to_delete->segmentos) > 1){
								segmento_t* segmento = list_get(tabla_de_proceso_to_delete->segmentos,list_size(tabla_de_proceso_to_delete->segmentos)- 1);
								int id_segmento = segmento->ids;
								if(id_segmento != 0) {
									eliminar_segmento(tabla_de_proceso_to_delete, lista_de_huecos_libres, id_segmento);
								}
							}
							list_remove_element(lista_de_tablas, tabla_de_proceso_to_delete);
							free(tabla_de_proceso_to_delete->segmentos);
							free(tabla_de_proceso_to_delete);
							log_info(logger,"Eliminacion del Proceso PID: %d", nueva_instruccion->pid);
							break;
						case ALLOCATE_SEGMENT:
							log_info(logger,"Creacion del Proceso PID: %d", nueva_instruccion->pid);
							allocate_segmento_0(nueva_instruccion->pid);
							break;
						case PRINT_SEGMENTS:
							log_info(logger,"Solicitud de impresion PID: %d", nueva_instruccion->pid);
							imprimir_tabla_segmentos();
							break;
						case PRINT_MEMORY_DATA:
							log_info(logger,"Solicitud de impresion de datos PID: %d", nueva_instruccion->pid);
							imprimir_datos();
							break;
						default:
							log_error(logger, "Instruccion desconocida");
							break;
				}


				free(nueva_instruccion->param1);
				free(nueva_instruccion->param2);
				free(nueva_instruccion->param3);
				free(nueva_instruccion);
				break;
			case 2:
				serializar_tabla_segmentos(server_connection,lista_de_tablas);
				break;
			case 3:
				log_info(logger,"Solicitud de COMPACTACION recibida");
				compactar_memoria();
				log_info(logger,"Resultado de COMPACTACION");
				imprimir_tabla_segmentos();
				t_resp_mem estado_memoria = COMPACTATION_SUCCESS;
				serializar_respuesta_memoria_kernel(server_connection, estado_memoria);
				break;
			default:
				exit_status=1;
				break;
			}

			free(paquete->buffer->stream);
			free(paquete->buffer);
			free(paquete);
		}
}
