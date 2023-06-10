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
						//responder a kernel
						break;
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
