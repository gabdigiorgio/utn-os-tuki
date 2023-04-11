/*
 ============================================================================
 Name        : memoria.c
 Author      : 
 Version     :
 Copyright   : Your copyright notice
 Description : Hello World in C, Ansi-style
 ============================================================================
 */

#include "../includes/memoria.h"



int main(int argc, char *argv[]) {
	logger = iniciar_logger();
	  if (argc < 2) {
	    log_error(logger, "Falta parametro del path del archivo de configuracion");
	    return EXIT_FAILURE;
	  }
	config = iniciar_config(argv[1]);

	//Inicializamos las variables globales desde el config, que loggee errores o success si todo esta bien
	int exit_status = initial_setup();
	if (exit_status==EXIT_FAILURE){
		return EXIT_FAILURE;
	}

	int server_connection = iniciar_servidor(server_port);

	log_info(logger, "Memoria lista para recibir al CPU, Kernel o File System");
	//SO_REUSEADDR flag para reutilizar el socket

	while (num_threads < 4){
		t_conexion *conexion = malloc(sizeof *conexion);
		conexion->num_socket = esperar_cliente(server_connection);
		conexion->id_cliente = num_threads;

		pthread_create(&(tid[num_threads]), NULL, (void *)thread_main, conexion);
		pthread_detach(&(tid[num_threads]));
		num_threads++;
	}

	log_info(logger, "Se superaron las conexiones maximas establecidas, cerrando memoria");
	liberar_conexion(server_connection);
}

void thread_main(t_conexion *conexion){
	int estado = 1;
	log_info(logger, "Thread iniciado correctamente");
	log_info(logger,handshake(conexion->num_socket));
	t_list* lista;
	while (estado == 1) {
			int cod_op = recibir_operacion(conexion->num_socket);
			switch (cod_op) {
			case MENSAJE:
				recibir_mensaje(conexion->num_socket);
				break;
			case PAQUETE:
				lista = recibir_paquete(conexion->num_socket);
				log_info(logger, "Me llegaron los siguientes valores:\n");
				list_iterate(lista, (void*) iterator);
				break;
			case -1:
				log_error(logger, "el cliente se desconecto. Terminando servidor");
				num_threads--;
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

