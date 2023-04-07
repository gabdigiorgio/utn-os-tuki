/*
 ============================================================================
 Name        : kernel.c
 Author      : 
 Version     :
 Copyright   : Your copyright notice
 Description : Hello World in C, Ansi-style
 ============================================================================
 */


#include "../includes/kernel.h"


t_log* logger;

int server_connection;

int memoria_connection;
int file_system_connection;
int cpu_connection;


int main(void) {

	//Iniciamos tanto el log como el config
	logger = iniciar_logger();
	config = iniciar_config();

	//Inicializamos las variables globales desde el config, que loggee errores o success si todo esta bien
	log_info(logger,initial_setup());

	// Nos conectamos a los "servidores" (memoria, file system y CPU) como "clientes"
	if((memoria_connection = crear_conexion(memoria_ip,memoria_port)) != 0) log_info(logger, "Conexion establecida con la Memoria");
	if((file_system_connection = crear_conexion(file_system_ip,file_system_port)) != 0) log_info(logger, "Conexion establecida con el File System");
	if((cpu_connection = crear_conexion(cpu_ip,cpu_port)) != 0) log_info(logger, "Conexion establecida con la CPU");

	//NO HAGO HANDSHAKE con ninguno de los 3 (no lo pide la consigna)

	// Inicio servidor del Kernel
	int socket_servidor = iniciar_servidor(server_port);
	log_info(logger, "Kernel listo para recibir Consolas");

	// Espero Conexiones de las consolas
	while (1){
		 pthread_t console_thread;
		   int *socket_console_client = malloc(sizeof(int));
		   *socket_console_client = esperar_cliente(socket_servidor);
		   pthread_create(&console_thread, NULL, (void*) atender_consola, socket_console_client);
		   pthread_detach(console_thread);
	}



	terminar_programa();

	return EXIT_SUCCESS;
}


void terminar_programa()
{
	log_destroy(logger);
	config_destroy(config);
	liberar_conexion(memoria_connection);
	liberar_conexion(file_system_connection);
	liberar_conexion(cpu_connection);
}
