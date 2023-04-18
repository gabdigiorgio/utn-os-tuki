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

	//Iniciamos log, config, cant_threads_activos y listas pcb
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
	cant_threads_activos = 0;
	iniciar_pcb_lists();

	// Nos conectamos a los "servidores" (memoria, file system y CPU) como "clientes"
	// IMPORTANTE!! -> es probable que las siguientes conexiones tengan que ser manejadas mediante hilos e finalizar el kernel si pierde la conexion con alguno de estos
	if((memoria_connection = crear_conexion(memoria_ip,memoria_port)) == 0 || handshake(memoria_connection,2,4) == -1) {
		terminar_programa();
		return EXIT_FAILURE;
	}
	if((cpu_connection = crear_conexion(cpu_ip,cpu_port)) == 0 || handshake(cpu_connection,2,1) == -1) {
		terminar_programa();
		return EXIT_FAILURE;
	}
	if((file_system_connection = crear_conexion(file_system_ip,file_system_port)) == 0 || handshake(file_system_connection,2,3) == -1) {
		terminar_programa();
		return EXIT_FAILURE;
	}

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

void iniciar_pcb_lists(){
	pcb_ready_list = list_create();
	pcb_new_list = list_create();
	pcb_suspended_list = list_create();
}



pcb_t *crear_proceso(pcb_t *pcb){
	pcb_t *proceso = malloc(sizeof(pcb_t));
	//Desde aqui se asignarian los tiempos para manejar los algoritmos de planificacion asignando los que inician en 0 y el estado como new
	return proceso;
}

void agregar_pcb_a_new(pcb_t *pcb,int socket_consola){
	pcb_t *proceso = crear_proceso(pcb);
	list_add(pcb_new_list,proceso);
	sleep(1);
	printf("PID = [%d] ingresa a NEW", proceso->pid);
	sleep(20);
	log_info(logger,"Proceso añadido correctamente");
}
void iniciar_planificador_largo_plazo(){
	pthread_create(&pcb_new,NULL,(void *)agregar_pcb_a_new,NULL);
	pthread_detach(pcb_new);
}
void terminar_programa()
{
	log_destroy(logger);
	config_destroy(config);
	liberar_conexion(memoria_connection);
	liberar_conexion(file_system_connection);
	liberar_conexion(cpu_connection);
}
