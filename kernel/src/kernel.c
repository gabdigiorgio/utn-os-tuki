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

int main(int argc, char *argv[])
{

	// ---------------------------------------------

	//Iniciamos log, config, cant_threads_activos y listas pcb

	logger = iniciar_logger();

	if (argc < 2)
	{
		log_error(logger, "Falta parametro del path del archivo de configuracion");
		return EXIT_FAILURE;
	}

	config = iniciar_config(argv[1]);

	int exit_status = initial_setup(); //Inicializamos las variables globales desde el config, que loggee errores o success si todo esta bien
	if (exit_status == EXIT_FAILURE)
	{
		return EXIT_FAILURE;
	}

	cant_threads_activos = 0;

	pid = 0;

	iniciar_pcb_lists();

	iniciar_semaforos();

	iniciar_lista_recursos();

	iniciar_planificador_corto_plazo();

	iniciar_planificador_largo_plazo();

	// ---------------------------------------------

	// Nos conectamos a los "servidores" (memoria, file system y CPU) como "clientes"

	if ((memoria_connection = crear_conexion(memoria_ip, memoria_port)) == 0 || handshake(memoria_connection, 2, 4) == -1)
	{
		terminar_programa();
		return EXIT_FAILURE;
	}
	if ((cpu_connection = crear_conexion(cpu_ip, cpu_port)) == 0 || handshake(cpu_connection, 2, 1) == -1)
	{
		terminar_programa();
		return EXIT_FAILURE;
	}
	if ((file_system_connection = crear_conexion(file_system_ip, file_system_port)) == 0 || handshake(file_system_connection, 2, 3) == -1)
	{
		terminar_programa();
		return EXIT_FAILURE;
	}

	// ---------------------------------------------

	// Inicio servidor del Kernel
	int socket_servidor = iniciar_servidor(server_port);
	log_info(logger, "Kernel listo para recibir Consolas");

	// ---------------------------------------------

	// Espero Conexiones de las consolas
	pthread_t console_thread;
	pthread_create(&console_thread, NULL, (void*) atender_consola, socket_servidor);

	pthread_join(console_thread, NULL);

	//hilo para enviar contexto a cpu
	pthread_t cpu_thread;
	//pthread_create(&cpu_thread,NULL,(void*)enviar_contexto,socket_servidor);

	terminar_programa();

	recibido = 0;

	return EXIT_SUCCESS;
}

void iniciar_semaforos()
{
	sem_init(&sem_grado_multi, 0, grado_max_multiprogramacion);
	sem_init(&sem_estado_new, 0, 0);
	sem_init(&sem_estado_ready, 0, 0);
	sem_init(&sem_estado_block, 0, 0);
	sem_init(&sem_exec_libre, 0, 1); // Se pone 1 para indicar que exec esta libre desde un principio
	sem_init(&sem_estado_exec, 0, 0);
	sem_init(&sem_estado_exit, 0, 0);
	sem_init(&sem_pid_aumento, 0, 1);
}

void iniciar_pcb_lists()
{
	pcb_ready_list = init_list_mutex();
	pcb_new_list = init_list_mutex();
	pcb_block_list = init_list_mutex();
	pcb_exit_list = init_list_mutex();
}

void terminar_programa()
{
	log_destroy(logger);
	config_destroy(config);
	list_mutex_destroy(pcb_new_list);
	list_mutex_destroy(pcb_ready_list);
	list_mutex_destroy(pcb_block_list);
	list_mutex_destroy(pcb_exit_list);
	sem_destroy(&sem_estado_exec);
	sem_destroy(&sem_estado_exit);
	liberar_conexion(memoria_connection);
	liberar_conexion(file_system_connection);
	liberar_conexion(cpu_connection);
}

void iniciar_lista_recursos()
{
	int cant_lista_nombre_recursos = string_array_size(lista_nombre_recursos);
	int cant_lista_instancias_recursos = string_array_size(lista_instancias_recursos);
	if (cant_lista_nombre_recursos != cant_lista_instancias_recursos)
	{
		log_error(logger, "No todos los recursos definidos tiene instancias");
		exit(1);
	}

	lista_recursos = init_list_mutex();
	for (int i = 0; i < cant_lista_nombre_recursos; i++)
	{
		t_recurso *recurso = malloc(sizeof(t_recurso));
		recurso->id = i;
		recurso->nombre_recurso = lista_nombre_recursos[i];
		recurso->instancias = (*lista_instancias_recursos[i]) - '0';
		pthread_mutex_init(&(recurso->mutex_instancias), NULL);
		recurso->cola_bloqueados = init_list_mutex();
		list_add(lista_recursos->lista, recurso);

	}
}
