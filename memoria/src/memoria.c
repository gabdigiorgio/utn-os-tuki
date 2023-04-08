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

char* ip;
char* server_port;
char* client_port;
t_log* logger;
t_config* config;
int server_connection;
int client_connection;
int num_threads = 0;
pthread_t tid[3];

int main(void) {
	logger = iniciar_logger();
	config = iniciar_config();

	log_info(logger,initial_setup());

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
char* initial_setup(){
	char* result = "";
	int error = 1;

	if(error == 1 && strcmp(server_port = config_get_string_value(config,"PUERTO_ESCUCHA"),"") == 0){
		result = "No se pudo obtener el puerto de escucha desde el archivo config";
		error = 0;
	}
	if(error == 1) result = "Valores de configuracion leidos correctamente";

	return result;
}
