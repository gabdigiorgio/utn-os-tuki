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



int main(int argc, char *argv[]) {

	//Iniciamos log, config y cant_threads_activos
	logger = iniciar_logger();
	  if (argc < 2) {
	    log_error(logger, "Falta parametro del path del archivo de configuracion");
	    return EXIT_FAILURE;
	  }
	config = iniciar_config(argv[1]);
	cant_threads_activos = 0;

	//Inicializamos las variables globales desde el config, que loggee errores o success si todo esta bien
	int exit_status = initial_setup();
	if (exit_status==EXIT_FAILURE){
		return EXIT_FAILURE;
	}

	// Nos conectamos a los "servidores" (memoria, file system y CPU) como "clientes"
	// IMPORTANTE!! -> es probable que las siguientes conexiones tengan que ser manejadas mediante hilos e finalizar el kernel si pierde la conexion con alguno de estos
	if((memoria_connection = crear_conexion(memoria_ip,memoria_port)) != 0) log_info(logger, "Conexion establecida con la Memoria");
	if((file_system_connection = crear_conexion(file_system_ip,file_system_port)) != 0) log_info(logger, "Conexion establecida con el File System");
	if((cpu_connection = crear_conexion(cpu_ip,cpu_port)) != 0) log_info(logger, "Conexion establecida con la CPU");

	//NO HAGO HANDSHAKE con ninguno de los 3 (no lo pide la consigna)

	// Inicio servidor del Kernel
	int socket_servidor = iniciar_servidor(server_port);
	log_info(logger, "Kernel listo para recibir Consolas");

	// Espero Conexiones de las consolas
	while (1){
		if(cant_threads_activos<CANTIDAD_DE_THREADS){
		  pthread_t console_thread;

		   int *socket_console_client = malloc(sizeof(int));
		   *socket_console_client = esperar_cliente(socket_servidor);

		   pthread_create(&console_thread, NULL, (void*) atender_consola, socket_console_client);
		   pthread_detach(console_thread);

		   cant_threads_activos++;
		}
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
