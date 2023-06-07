#include "../includes/comm_threadMem.h"

void conexion_kernel(int server_connection){

	log_info(logger, "Memoria lista para recibir mensajes del Kernel");
		kernel_connection= esperar_cliente(server_connection);
		log_info(logger, handshake(kernel_connection));

		while (1)
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
					//responder a kernel
				}


				free(nueva_instruccion->param1);
				free(nueva_instruccion->param2);
				free(nueva_instruccion->param3);
				free(nueva_instruccion);
				break;

			default:
				log_info(logger, "falle");
				break;
			}
		}
}
