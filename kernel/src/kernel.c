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

	iniciar_planificador_corto_plazo();

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

void terminar_programa()
{
	log_destroy(logger);
	config_destroy(config);
	liberar_conexion(memoria_connection);
	liberar_conexion(file_system_connection);
	liberar_conexion(cpu_connection);
}

void iniciar_planificador_corto_plazo(){

	pthread_t hilo_ready;
	pthread_create(&hilo_ready, NULL, (void *)estado_ready, NULL);
	pthread_detach(hilo_ready);
}

void estado_ready() {
	while(1){

		pcb_t* pcb_a_ejecutar = NULL;

		if (!list_is_empty(pcb_new_list)){
			pcb_t* pcb_nuevo = list_pop(pcb_new_list);
			list_add(pcb_ready_list, pcb_nuevo);
		}

		if(strcmp(algoritmo_planificacion, "FIFO") == 0){
			pcb_a_ejecutar = list_pop(pcb_ready_list);
		}
		else if (strcmp(algoritmo_planificacion, "HRRN") == 0){
			pcb_a_ejecutar = planificar_hrrn();

			list_remove_by_condition(pcb_ready_list, (void*)mismo_pcb, pcb_a_ejecutar);
		}
	}
}

bool mismo_pcb(pcb_t* pcb1, pcb_t* pcb2) {
    return (pcb1->pid == pcb2->pid);
}

pcb_t* planificar_hrrn(){
	pcb_t* pcb_a_ejecutar = NULL;
	float ratio_actual = 0;

	for(int i = 0; i < list_size(pcb_ready_list); i++){
		pcb_t* pcb_actual = list_get(pcb_ready_list, i);
		float ratio_respuesta = calcular_ratio(pcb_actual);

		if(ratio_respuesta > ratio_actual){
			ratio_actual = ratio_respuesta;
			pcb_a_ejecutar = pcb_actual;
		}
	}
	return pcb_a_ejecutar;
}

float calcular_ratio(pcb_t* pcb_actual){
	float ratio = (pcb_actual->tiempo_llegada_ready + pcb_actual->estimado_proxima_rafaga)/pcb_actual->estimado_proxima_rafaga;
	return ratio;
}
