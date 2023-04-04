/*
 ============================================================================
 Name        : cpu.c
 Author      : 
 Version     :
 Copyright   : Your copyright notice
 Description : Hello World in C, Ansi-style
 ============================================================================
 */

#include "cpu.h"

char* ip;
char* server_port;
char* client_port;
t_log* logger;
t_config* config;
int server_connection;
int client_connection;


int main(void) {

	//Iniciamos tanto el log como el config
	logger = iniciar_logger();
	config = iniciar_config();
	//Inicializamos las variables globales desde el config, que loggee errores o success si todo esta bien
	log_info(logger,initial_setup());

	if(client_connection = crear_conexion(ip,client_port) != 0) log_info(logger, "Conexion con la memoria establecida correctamente");

	int server_connection = iniciar_servidor(server_port);

	log_info(logger, "Cpu lista para recibir al Kernel");
	int connection_fd = esperar_cliente(server_connection);
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

char* initial_setup(){
	char* result = "";
	int error = 1;

	if(error == 1 && strcmp(ip = config_get_string_value(config,"IP_MEMORIA"),"") == 0){
		result = "No se pudo obtener la IP desde el archivo config";
		error = 0;
	}
	if(error == 1 && strcmp(server_port = config_get_string_value(config,"PUERTO_ESCUCHA"),"") == 0){
		result = "No se pudo obtener el puerto de escucha desde el archivo config";
		error = 0;
	}
	if(error == 1 && strcmp(client_port = config_get_string_value(config,"PUERTO_MEMORIA"),"") == 0){
		result = "No se pudo obtener el puerto de conexion desde el archivo config";
		error = 0;
	}
	if(error == 1) result = "Valores de configuracion leidos correctamente";

	return result;
}
