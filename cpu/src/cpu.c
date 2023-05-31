/*
 ============================================================================
 Name        : cpu.c
 Author      : 
 Version     :
 Copyright   : Your copyright notice
 Description : Hello World in C, Ansi-style
 ============================================================================
 */

#include "../includes/cpu.h"

int main(int argc, char *argv[]) {
	//Iniciamos tanto el log como el config
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

	if((memoria_connection = crear_conexion(memoria_ip,memoria_port)) == 0 || handshake_cliente(memoria_connection,1,4) == -1) {
		terminar_programa();
		return EXIT_FAILURE;
	}

	int server_connection = iniciar_servidor(server_port);

	conexion_kernel(server_connection);

	return EXIT_SUCCESS;
}

void iterator(char* value) {
	log_info(logger,"%s", value);
}
int registro_4b(char registro[5]){
	if(strlen(registro)!= 4){
		return EXIT_FAILURE;
	}
	return 0;
}

int registro_8b(char registro[9]){
	if(strlen(registro)!= 8){
		return EXIT_FAILURE;
	}
	return 0;
}
int registro_16b(char registro[16]){
	if(strlen(registro)!= 16){
		return EXIT_FAILURE;
	}
	return 0;
}

void terminar_programa()
{
	log_destroy(logger);
	config_destroy(config);
	liberar_conexion(memoria_connection);
}


