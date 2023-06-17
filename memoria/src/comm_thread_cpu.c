#include "../includes/comm_thread_cpu.h"

void conexion_cpu(int server_connection)
{

	log_info(logger, "Memoria lista para recibir al CPU");
	cpu_connection = esperar_cliente(server_connection);

	int exit_status = 0;

	while (exit_status == 0)
	{
		t_paquete *paquete = malloc(sizeof(t_paquete));
		paquete->buffer = malloc(sizeof(t_buffer));
		deserializar_header(paquete, server_connection);

		switch (paquete->codigo_operacion)
		{
		case 1:
			t_instruc_mem *nueva_instruccion = inicializar_instruc_mem();
			deserializar_instruccion_memoria(nueva_instruccion, paquete->buffer, paquete->lineas);
			int direccion_fisica;
			switch (nueva_instruccion->estado)
			{

			case MOV_IN:
				log_info(logger, "MOV_IN llego a memoria");
				int valor;
				direccion_fisica = atoi(nueva_instruccion->param2);
				memcpy(&valor, memoria + direccion_fisica, sizeof(int));
				snprintf(nueva_instruccion->param2, sizeof(nueva_instruccion->param2), "%d", valor);
				serializar_instruccion_memoria(server_connection, nueva_instruccion);
				break;
			case MOV_OUT:
				log_info(logger, "MOV_OUT llego a memoria");
				direccion_fisica = atoi(nueva_instruccion->param1);
				memcpy(memoria + direccion_fisica, nueva_instruccion->param2, nueva_instruccion->param2_length);
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
