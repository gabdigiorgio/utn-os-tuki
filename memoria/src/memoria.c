/*
 ============================================================================
 Name        : memoria.c
 Author      : 
 Version     :
 Copyright   : Your copyright notice
 Description : Hello World in C, Ansi-style
 ============================================================================
 */

#include "memoria.h"

int main(void) {
	logger = log_create("memoria.log", "Memoria", 1, LOG_LEVEL_DEBUG);
	int memoria_fd = iniciar_servidor();
	log_info(logger, "Memoria lista para recibir al CPU, Kernel o File System");
	int connection_fd = esperar_cliente(memoria_fd);
	t_list* lista;
	while (1) {
			int cod_op = recibir_operacion(connection_fd);
			switch (cod_op) {
			case MENSAJE:
				recibir_mensaje(connection_fd);
				break;
			case PAQUETE:
				lista = recibir_paquete(connection_fd);
				log_info(logger, "Me llegaron los siguientes valores:\n");
				list_iterate(lista, (void*) iterator);
				break;
			case -1:
				log_error(logger, "el cliente se desconecto. Terminando servidor");
				return EXIT_FAILURE;
			default:
				log_warning(logger,"Operacion desconocida. No quieras meter la pata");
				break;
			}
		}
		return EXIT_SUCCESS;
}
void iterator(char* value) {
	log_info(logger,"%s", value);
}
