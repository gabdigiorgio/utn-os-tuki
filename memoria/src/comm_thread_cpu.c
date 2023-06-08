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
				//logica temporal hasta tener la que va
				//pthread_t thread_m_block;
				//pthread_create(&thread_m_block, NULL, (void*) m_block, (t_m_block_args*) args);
				//pthread_join(thread_m_block);
				log_info(logger, "MOV_IN llego a memoria");
				//enviar_contexto(pcb);
				//cambiar por la correcta
				break;
			case MOV_OUT:
				//logica temporal hasta tener la que va
				//pthread_t thread_m_block;
				//pthread_create(&thread_m_block, NULL, (void*) m_block, (t_m_block_args*) args);
				//pthread_join(thread_m_block);
				log_info(logger, "MOV_OUT llego a memoria");
				//enviar_contexto(pcb);
				//cambiar por la correcta
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
