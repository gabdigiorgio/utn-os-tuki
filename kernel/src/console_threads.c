/*
 * threads.c
 *
 *  Created on: Apr 7, 2023
 *      Author: utnso
 */

#include "../includes/console_threads.h"

void atender_consola(int *socket_console_client){
	int estado = 1;
	log_info(logger, "Thread iniciado correctamente");
	t_list* lista;
	while (estado == 1) {
			int cod_op = recibir_operacion(*socket_console_client);
			switch (cod_op) {
			case MENSAJE:
				recibir_mensaje(*socket_console_client);
				break;
			case PAQUETE:
				lista = recibir_paquete(*socket_console_client);
				log_info(logger, "Me llegaron los siguientes valores:\n");
				list_iterate(lista, (void*) iterator);
				break;
			case -1:
				log_error(logger, "el cliente se desconecto. Terminando servidor");
				estado = 0;
				break;
			default:
				log_warning(logger,"Operacion desconocida. No quieras meter la pata");
				break;
			}
		}
}

void iterator(char* value) {
	log_info(logger,"%s", value);
}
