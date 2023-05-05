#include "../includes/comm_thread.h"

void conexion_kernel(int server_connection){

	log_info(logger, "Cpu lista para recibir al Kernel");
	int connection_fd = esperar_cliente(server_connection);
	log_info(logger,handshake(connection_fd));

	while (1) {
		//Reservo memoria para el paquete
		t_paquete* paquete = malloc(sizeof(t_paquete));
		paquete->buffer = malloc(sizeof(t_buffer));

		//Recivo el header del paquete + el stream de datos
		deserializar_header(paquete, connection_fd);

		//Reviso el header para saber de que paquete se trata y deserealizo acorde
		switch(paquete->codigo_operacion){
			case 1:
				contexto = deserializar_contexto(paquete->buffer, paquete->lineas);
				int result = ejecutar_contexto(paquete->lineas);
				armar_contexto();
				log_info(logger, "El proceso: %d llego a CPU", contexto->pid);
				log_info(logger, "El proceso sale con un delay %d", contexto->delay);
				log_info(logger,contexto->registros->ax);
				serializar_contexto(connection_fd,contexto);
				break;
			default:
				break;
		}
	}
}
