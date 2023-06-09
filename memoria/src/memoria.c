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


	//Inicializamos las variables globales desde el config, que loggee errores o success si todo esta bien

	if (argc < 2) {
	  log_error(logger, "Falta parametro del path del archivo de configuracion");
	  return EXIT_FAILURE;
	}

	config = iniciar_config(argv[1]);


	int exit_status = initial_setup();

	if (exit_status==EXIT_FAILURE){
		return EXIT_FAILURE;
	}

	// Asigno tamaño a la memoria, creo lista de tablas de segmentos, de huecos_libres y segmento 0

	memoria = malloc(tam_memoria);

	lista_de_tablas = list_create();

	segemento_0.ids=0;
	segemento_0.direccion_base=0;
	segemento_0.tamanio = tam_segmento_0;

	lista_de_huecos_libres = list_create();
	hueco_libre_t *primer_hueco_libre = malloc(sizeof(hueco_libre_t));
	primer_hueco_libre->direccion_base = segemento_0.tamanio;
	primer_hueco_libre->tamanio = tam_memoria - segemento_0.tamanio;
	list_add(lista_de_huecos_libres,primer_hueco_libre);


	// Esperamos conexiones de Kernel, CPU y File-System

	int server_connection = iniciar_servidor(server_port);

	cpu_conectada = false;
	kernel_conectado = false;
	fileSystem_conectado = false;
	log_info(logger, "Memoria lista para recibir al CPU, Kernel o File System");
	//create_tabla_segmento(1);
	//crear_segmento(1,1,1,1);
	//log_info(logger,"Segmento creado correctamente");
	//SO_REUSEADDR flag para reutilizar el socket

	while (1){
		if(num_threads<CANTIDAD_DE_THREADS){
			t_conexion *conexion = malloc(sizeof *conexion);
			conexion->num_socket = esperar_cliente(server_connection);
			conexion->id_cliente = num_threads;

			pthread_create(&(tid[num_threads]), NULL, (void *)thread_main, conexion);
			pthread_detach(&(tid[num_threads]));
			num_threads++;
		}
	}


	log_info(logger, "Se superaron las conexiones maximas establecidas, cerrando memoria");
	liberar_conexion(server_connection);


}

void thread_main(t_conexion *conexion){
	int estado = 1;
	log_info(logger, "Thread iniciado correctamente");
	char* mensaje=handshake(conexion->num_socket);
	log_info(logger,mensaje);

	if(strcmp(mensaje, "Handshake de CPU recibido correctamente")==0){

		  //conexion_cpu(conexion->num_socket);
	}
	else if(strcmp(mensaje, "Handshake de Kernel recibido correctamente")==0)
	{
		conexion_kernel(conexion->num_socket);
	}
	else if(strcmp(mensaje, "Handshake de File System recibido correctamente")==0)
	{
		//conexion_fileSystem(conexion->num_socket);
	}
}

void create_tabla_segmento(int pid) {
	tabla_segmentos_t *tabla = malloc(sizeof(tabla_segmentos_t));
	tabla->pid = pid;
	tabla->segmentos = list_create();
	log_info(logger,"%d",list_size(lista_de_tablas));
	log_info(logger,"%d",list_size(tabla->segmentos));
	list_add(lista_de_tablas,tabla);
	log_info(logger,"%d",list_size(lista_de_tablas));

}


void iterator(char* value) {
	log_info(logger,"%s", value);
}


