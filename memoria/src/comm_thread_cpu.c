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

			switch (nueva_instruccion->estado)
			{

			case MOV_IN:
				log_info(logger, "MOV_IN llego a memoria");
				//fijarse la direccion (param2)
				//devolver el valor que esta dentro de ese registro a CPU
				break;
			case MOV_OUT:
				log_info(logger, "MOV_OUT llego a memoria");
				//leer el valor del registro (param1)
				//calcular direccion fisica a partir de la logica (param2)
				//escribir en la direccion fisica el registro
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
