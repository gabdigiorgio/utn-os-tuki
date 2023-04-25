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

	// ---------------------------------------------

	//Iniciamos log, config, cant_threads_activos y listas pcb

	logger = iniciar_logger();

	if (argc < 2) {
		log_error(logger, "Falta parametro del path del archivo de configuracion");
		return EXIT_FAILURE;
	 }

	config = iniciar_config(argv[1]);

	int exit_status = initial_setup(); //Inicializamos las variables globales desde el config, que loggee errores o success si todo esta bien
	if (exit_status==EXIT_FAILURE){
		return EXIT_FAILURE;
	}

	cant_threads_activos = 0;

	iniciar_pcb_lists();

	//iniciar_planificador_corto_plazo();

	// ---------------------------------------------

	// Nos conectamos a los "servidores" (memoria, file system y CPU) como "clientes"

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

	// ---------------------------------------------

	// Inicio servidor del Kernel
	int socket_servidor = iniciar_servidor(server_port);
	log_info(logger, "Kernel listo para recibir Consolas");
	//Se proceden a iniciar la estructuras necesarias para la planificacion
	iniciar_pcb_lists();
	iniciar_planificador_largo_plazo();
	// ---------------------------------------------

	// Espero Conexiones de las consolas
	pthread_t console_thread;
	pthread_create(&console_thread, NULL, (void*) atender_consola, socket_servidor);

	pthread_join(console_thread, NULL);

	terminar_programa();

	recibido = 0;

	return EXIT_SUCCESS;
}




void iniciar_pcb_lists(){
	pcb_ready_list = init_list_mutex();
	pcb_new_list = init_list_mutex();
	pcb_block_list = init_list_mutex();
}



pcb_t *crear_proceso(uint32_t largo,t_list* instrucciones){
	pcb_t *proceso = malloc(sizeof(pcb_t));
	proceso->pid= largo+1;
	proceso->estimado_proxima_rafaga=estimacion_inicial;
	proceso->instrucciones=instrucciones;
	//Desde aqui se asignarian los tiempos para manejar los algoritmos de planificacion asignando los que inician en 0 y el estado como new
	return proceso;
}


void agregar_pcb_a_new(int socket_consola,t_list* instrucciones){
	uint32_t largo = list_size(pcb_new_list->lista);
	pcb_t *proceso = crear_proceso(largo,instrucciones);
	sem_wait(&mutex_list_new);
	list_push(pcb_new_list,proceso);
	sem_post(&mutex_list_new);
	printf("PID = [%d] ingresa a NEW", proceso->pid);
	sem_post(&admitir_pcb);

}


void iniciar_planificador_largo_plazo(){
	sem_init(&sem_grado_multiprogramacion,0,get_grado_multiprogramacion());
	sem_init(&mutex_list_new,0,1);
	pthread_create(&pcb_new,NULL,(void *)agregar_pcb_a_new,NULL);
	pthread_detach(pcb_new);
}


void terminar_programa()
{
	log_destroy(logger);
	config_destroy(config);
	list_mutex_destroy(pcb_new_list);
	list_mutex_destroy(pcb_ready_list);
	list_mutex_destroy(pcb_block_list);
	liberar_conexion(memoria_connection);
	liberar_conexion(file_system_connection);
	liberar_conexion(cpu_connection);
}

void iniciar_planificador_corto_plazo(){

	pthread_t hilo_ready;
	pthread_t hilo_block;
	pthread_create(&hilo_ready, NULL, (void *)estado_ready, NULL);
	pthread_create(&hilo_block,NULL,(void*)estado_block,NULL);
	pthread_detach(hilo_ready);
	pthread_detach(hilo_block);
}

void estado_ready() {
	while(1){
		sem_wait(&admitir_pcb);
		sem_wait(&sem_grado_multiprogramacion);
		pcb_t* pcb_a_ejecutar;

		if (!list_is_empty(pcb_new_list->lista)){
			sem_wait(&mutex_list_new);
			pcb_t* pcb_nuevo = list_pop(pcb_new_list);
			sem_post(&mutex_list_new);
			list_push(pcb_ready_list, pcb_nuevo);
		}

		if(strcmp(algoritmo_planificacion, "FIFO") == 0){
			pcb_a_ejecutar = list_pop(pcb_ready_list);
			enviar_proceso_a_ejecutar(pcb_a_ejecutar);
		}
		else if (strcmp(algoritmo_planificacion, "HRRN") == 0){
			//list_add_sorted(pcb_exec_list, pcb_a_ejecutar, mayor_ratio);
			// pcb_a_ejecutar = list_pop(pcb_ready_list);
		}
	}
}


void enviar_proceso_a_ejecutar(pcb_t* pcb_a_ejecutar){

/*
 *
 * 		wait(sem_exec == 0)
 *
 * 		serializar_contexto()
 *
 * 		recibir_contexto()
 *
 *      cosas
 *
 *      signal(sem_exec) (reduce el semasforo a 0)
 *
 *
 */
}


//void estado_exec(){
//	while(1)
//	{
//		if(!list_is_empty(pcb_exec_list)){
//			pcb_t* proceso_ejecutando=list_pop(pcb_exec_list);
//			proceso_ejecutando->estado=PCB_EXEC;
//			//ejecutar proceso
//			proceso_ejecutando->estado = PCB_EXIT;
//
//			sem_post(&sem_grado_multiprogramacion);
//		}
//	}
//}


void estado_block(){
	while(1){

	}
}
int get_grado_multiprogramacion(){
	return config_get_int_value(config,"GRADO_MAX_MULTIPROGRAMACION");
}
float calcular_ratio(pcb_t* pcb_actual){
	float ratio = (pcb_actual->tiempo_llegada_ready + pcb_actual->estimado_proxima_rafaga)/pcb_actual->estimado_proxima_rafaga;
	return ratio;
}

bool mayor_ratio(void* proceso_1, void* proceso_2){
	float ratio_1 = calcular_ratio((pcb_t*)proceso_1);
	float ratio_2 = calcular_ratio((pcb_t*)proceso_2);

	return ratio_1 > ratio_2;
}
