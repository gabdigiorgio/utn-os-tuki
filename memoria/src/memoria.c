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

	//Prueba huecos y eliminar segmentos
	//---------------------------------

	uint32_t memoria_total = 1500;

	segmento_t* segmento0 = malloc(sizeof(segmento_t));
	segmento_t* segmento1 = malloc(sizeof(segmento_t));
	segmento_t* segmento2 = malloc(sizeof(segmento_t));
	segmento_t* segmento3 = malloc(sizeof(segmento_t));
	segmento_t* segmento4 = malloc(sizeof(segmento_t));

	segmento0->ids = 0;
	segmento0->direccion_base = 0;
	segmento0->tamanio = 100;

	hueco_libre_t* hueco0 = malloc(sizeof(hueco_libre_t));
	hueco0->direccion_base = 100;
	hueco0->tamanio = 100;

	segmento1->ids = 1;
	segmento1->direccion_base = 200;
	segmento1->tamanio = 100;

	segmento2->ids = 2;
	segmento2->direccion_base = 300;
	segmento2->tamanio = 100;

	hueco_libre_t* hueco1 = malloc(sizeof(hueco_libre_t));
	hueco1->direccion_base = 400;
	hueco1->tamanio = 100;

	segmento3->ids = 3;
	segmento3->direccion_base = 500;
	segmento3->tamanio = 100;

	hueco_libre_t* hueco2 = malloc(sizeof(hueco_libre_t));
	hueco2->direccion_base = 600;
	hueco2->tamanio = 400;

	segmento4->ids = 4;
	segmento4->direccion_base = 1000;
	segmento4->tamanio = 200;

	hueco_libre_t* hueco3 = malloc(sizeof(hueco_libre_t));
	hueco3->direccion_base = 1200;
	hueco3->tamanio = 300;

	t_list* lista_segmentos = list_create();
	t_list* lista_huecos = list_create();

	list_add(lista_segmentos,segmento0);
	list_add(lista_segmentos,segmento1);
	list_add(lista_segmentos,segmento2);
	list_add(lista_segmentos,segmento3);
	list_add(lista_segmentos,segmento4);

	list_iterate(lista_segmentos,imprimir_valores_segmentos);

	list_add(lista_huecos,hueco0);
	list_add(lista_huecos,hueco1);
	list_add(lista_huecos,hueco2);
	list_add(lista_huecos,hueco3);

	list_iterate(lista_huecos,imprimir_valores_huecos);

	log_info(logger,"-----------------");

	eliminar_segmento(lista_segmentos,lista_huecos,2);

	list_iterate(lista_segmentos,imprimir_valores_segmentos);
	list_iterate(lista_huecos,imprimir_valores_huecos);

	log_info(logger,"-----------------");

	eliminar_segmento(lista_segmentos,lista_huecos,4);

	list_iterate(lista_segmentos,imprimir_valores_segmentos);
	list_iterate(lista_huecos,imprimir_valores_huecos);

	log_info(logger,"-----------------");

	eliminar_segmento(lista_segmentos,lista_huecos,3);

	list_iterate(lista_segmentos,imprimir_valores_segmentos);
	list_iterate(lista_huecos,imprimir_valores_huecos);

	log_info(logger,"-----------------");

	eliminar_segmento(lista_segmentos,lista_huecos,1);

	list_iterate(lista_segmentos,imprimir_valores_segmentos);
	list_iterate(lista_huecos,imprimir_valores_huecos);

	log_info(logger,"-----------------");

	eliminar_segmento(lista_segmentos,lista_huecos,0);

	list_iterate(lista_segmentos,imprimir_valores_segmentos);
	list_iterate(lista_huecos,imprimir_valores_huecos);

	log_info(logger,"-----------------");

	//---------------------------------

}
/*

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


} */

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


