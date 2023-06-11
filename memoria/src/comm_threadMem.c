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

				switch (nueva_instruccion->estado)
				{
				case CREATE_SEGMENT:
					log_info(logger, "Llego create_segmente a memoria");
					log_info(logger, "pid %d", nueva_instruccion->pid);

					int id_segmento = atoi(nueva_instruccion->param1);
					int tamanio_segmento = atoi(nueva_instruccion->param2);


					// ___ FIRST ____
					if (strcmp(algoritmo_asignacion, "FIRST") == 0){

						for (int i = 0; i < list_size(lista_de_huecos_libres); i++)
						{
							hueco_libre_t *hueco_libre = (hueco_libre_t*) list_get(lista_de_huecos_libres, i);


							if (hueco_libre->tamanio >= tamanio_segmento)
							{
								uint32_t base_segmento = hueco_libre->direccion_base;
								if(hueco_libre->tamanio - tamanio_segmento == 0){ // si el tamanio del hueco libre es 0, entonces lo elimino
									list_remove_element(lista_de_huecos_libres, hueco_libre);
								}
								else { //Si el tamanio del hueco libre no es 0, entonces modifico su base y su tamanio
									hueco_libre->direccion_base = hueco_libre->direccion_base + tamanio_segmento
									hueco_libre->tamanio = hueco_libre->tamanio - tamanio_segmento
								}

								crear_segmento(id_segmento,base_segmento, tamanio_segmento,nueva_instruccion->pid);
								break;
							}

						}

					}

					// ___ BEST ____
					else if (strcmp(algoritmo_asignacion, "BEST") == 0){

						int indice_hueco_mas_chico;
						int tamanio_menor;

						hueco_libre_t *hueco_libre = (hueco_libre_t*) list_get(lista_de_huecos_libres, 0);
						indice_hueco_mas_chico = 0;
						tamanio_menor = hueco_libre->tamanio;

						for (int i = 1; i < list_size(lista_de_huecos_libres); i++)
						{
							hueco_libre = (hueco_libre_t*) list_get(lista_de_huecos_libres, i);
							 if(hueco_libre->tamanio <tamanio_menor){
								 indice_hueco_mas_chico = i;
								 tamanio_menor = hueco_libre->tamanio;
							 }

						}

						hueco_libre = (hueco_libre_t*) list_get(lista_de_huecos_libres, indice_hueco_mas_chico);
						if (hueco_libre->tamanio >= tamanio_segmento)
						{

							uint32_t base_segmento = hueco_libre->direccion_base;
							if(hueco_libre->tamanio - tamanio_segmento == 0){ // si el tamanio del hueco libre es 0, entonces lo elimino
								list_remove_element(lista_de_huecos_libres, hueco_libre);
							}
							else { //Si el tamanio del hueco libre no es 0, entonces modifico su base y su tamanio
								hueco_libre->direccion_base = hueco_libre->direccion_base + tamanio_segmento
								hueco_libre->tamanio = hueco_libre->tamanio - tamanio_segmento
							}

							crear_segmento(id_segmento,base_segmento, tamanio_segmento,nueva_instruccion->pid);
						}

					}

					// ___ WORST ____
					else if (strcmp(algoritmo_asignacion, "WORST") == 0){

						int indice_hueco_mas_grande;
						int tamanio_mayor;

						hueco_libre_t *hueco_libre = (hueco_libre_t*) list_get(lista_de_huecos_libres, 0);
						indice_hueco_mas_grande = 0;
						tamanio_mayor = hueco_libre->tamanio;

						for (int i = 1; i < list_size(lista_de_huecos_libres); i++)
						{
							hueco_libre = (hueco_libre_t*) list_get(lista_de_huecos_libres, i);
							 if(hueco_libre->tamanio > tamanio_mayor){
								 indice_hueco_mas_grande = i;
								 tamanio_mayor = hueco_libre->tamanio;
							 }

						}

						hueco_libre = (hueco_libre_t*) list_get(lista_de_huecos_libres, indice_hueco_mas_grande);
						if (hueco_libre->tamanio >= tamanio_segmento)
						{

							uint32_t base_segmento = hueco_libre->direccion_base;
							if(hueco_libre->tamanio - tamanio_segmento == 0){ // si el tamanio del hueco libre es 0, entonces lo elimino
								list_remove_element(lista_de_huecos_libres, hueco_libre);
							}
							else { //Si el tamanio del hueco libre no es 0, entonces modifico su base y su tamanio
								hueco_libre->direccion_base = hueco_libre->direccion_base + tamanio_segmento
								hueco_libre->tamanio = hueco_libre->tamanio - tamanio_segmento
							}

							crear_segmento(id_segmento,base_segmento, tamanio_segmento,nueva_instruccion->pid);
						}
					}

					//responder a kernel


					// case CREATE_SEGMENT:
					// 	log_info(logger, "Llego create_segmente a memoria");
					// 	log_info(logger, "pid %d", nueva_instruccion->pid);
					// 	//responder a kernel
					// 	break;
					case ALLOCATE_SEGMENT:
						log_info(logger, "Llego un nuevo proceso a memoria");
						log_info(logger, "pid %d", nueva_instruccion->pid);
						allocate_segmento_0(nueva_instruccion->pid);
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
