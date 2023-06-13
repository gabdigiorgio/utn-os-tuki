#include "../includes/comm_threadMem.h"

void conexion_kernel(int server_connection){

	log_info(logger, "Memoria lista para recibir mensajes del Kernel");
	kernel_connection= esperar_cliente(server_connection);
	//log_info(logger, handshake(kernel_connection));
    int exit_status=0;
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

						log_info(logger, "Llego create_segmente a memoria");
						log_info(logger, "pid %d", pid);

						int id_segmento = atoi(nueva_instruccion->param1);
						int tamanio_segmento = atoi(nueva_instruccion->param2);

						int estado_memoria; // 0: Se puede crear segmento | 1: Out of memory | 2: Se necesita compactación

						if(tamanio_segmento > tam_memoria_restante){
							log_error(logger, "Out of Memory");
							estado_memoria = 1;

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

							if(estado_memoria == 0){
								tam_memoria_restante -= tamanio_segmento;
								log_info(logger, "Segmento %d creado | Proceso: %d", id_segmento, pid);
								log_info(logger, "Memoria Restante: %d", tam_memoria_restante);
							} else {
								log_info(logger, "Se requiere compactacion");
							}
						}


						//responder a kernel usando el numero devuelto por estado_memoria

						break;

						case DELETE_SEGMENT:
							log_info(logger, "Llego un nuevo proceso a memoria. Delete segment");
							log_info(logger, "pid %d", nueva_instruccion->pid);
							int id_segment = atoi(nueva_instruccion->param1);
							tabla_segmentos_t* tabla_de_proceso = buscar_tabla(pid);
							log_info(logger, "Segmento a borrar: %d | Tabla: %d", id_segment , tabla_de_proceso->pid);
							eliminar_segmento(tabla_de_proceso->segmentos, lista_de_huecos_libres, (uint32_t) id_segment);
							break;


						case ALLOCATE_SEGMENT:
							log_info(logger, "Llego un nuevo proceso a memoria");
							log_info(logger, "pid %d", nueva_instruccion->pid);
							allocate_segmento_0(nueva_instruccion->pid);
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
				log_info(logger, "Se solicito la tabla de segmentos");
				serializar_tabla_segmentos(server_connection,lista_de_tablas);
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
