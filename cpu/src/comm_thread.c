#include "../includes/comm_thread.h"

void conexion_kernel(int server_connection){

	log_info(logger, "Cpu lista para recibir al Kernel");
	int connection_fd = esperar_cliente(server_connection);
	log_info(logger,handshake(connection_fd));

	while (1) {
		contexto = malloc(sizeof(t_contexto));
		contexto->instrucciones = list_create();
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
				log_info(logger, "El numero de estado es: %d", contexto->estado);
				//log_info(logger, "El parametro de interrupcion es: %s", contexto->param); //aca esta el error
				//log_info(logger,contexto->registros->ax);
				log_info(logger,contexto->registros->ip);
				serializar_contexto(connection_fd,contexto);
				break;
			default:
				break;
		}

	    list_destroy_and_destroy_elements(contexto->instrucciones, free);
	    free(contexto->registros);
	    free(contexto->param);
	    free(contexto);
	}
}
