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

	iniciar_semaforos();

	iniciar_planificador_corto_plazo();

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

	//hilo para enviar contexto a cpu
	pthread_t cpu_thread;
	//pthread_create(&cpu_thread,NULL,(void*)enviar_contexto);


	terminar_programa();

	recibido = 0;


	return EXIT_SUCCESS;
}
/*t_contexto obtener_contexto_pcb(pcb_t pcb) {

	t_contexto *contexto = malloc(sizeof(t_contexto));
	t_registros *registros = malloc(sizeof(t_registros));
	memcpy(registros, &(pcb->registros_cpu), sizeof(t_registros));
	contexto->registros = registros;
	contexto->instrucciones = pcb->instrucciones;
	return contexto;
}*/

/*void enviar_contexto(){
	serializar_contexto(socket_servidor,contexto);
	//esperar respuesta de cpu
	//recv(socket_servidor, &(paquete->lineas), sizeof(uint32_t), 0);
	t_paquete* paquete = malloc(sizeof(t_paquete));
			paquete->buffer = malloc(sizeof(t_buffer));

			//Recivo el header del paquete + el stream de datos
			deserializar_header(paquete,socket_servidor);

	t_contexto* contexto_actualizado=deserializar_contexto(paquete->buffer, paquete->lineas);
	if(list_size(contexto->instrucciones) == list_size(contexto_actualizado->ip))
	{
		log_info(logger,"proceso a exit")
	}
	else
		log_info(logger,"proceso a block")
}*/

void iniciar_semaforos(){
	sem_init(&sem_estado_exec, 0, 1);
}

void iniciar_pcb_lists(){
	pcb_ready_list = init_list_mutex();
	pcb_new_list = init_list_mutex();
	pcb_block_list = init_list_mutex();
}



pcb_t *crear_proceso(uint32_t largo,t_list* instrucciones){
	pcb_t *proceso = malloc(sizeof(pcb_t));
	proceso->pid= largo+1;
	proceso->estimado_proxima_rafaga=config_get_int_value(config,"ESTIMACION_INICIAL");
	proceso->instrucciones=instrucciones;
	//Desde aqui se asignarian los tiempos para manejar los algoritmos de planificacion asignando los que inician en 0 y el estado como new
	return proceso;
}


/*void agregar_pcb_a_new(int socket_consola,t_list* instrucciones){
	uint32_t largo = list_size(pcb_new_list->lista);
	pcb_t *proceso = crear_proceso(largo,instrucciones);
	list_push(pcb_new_list,proceso);

	printf("PID = [%d] ingresa a NEW \n", proceso->pid);
	sem_post(&admitir_pcb);

}*/

void agregar_pcb_a_new(t_list* instrucciones){
	uint32_t largo = list_size(pcb_new_list->lista);
	pcb_t *proceso = crear_proceso(largo,instrucciones);
	list_push(pcb_new_list,proceso);
	sem_post(&sem_estado_new);
}


/*void iniciar_planificador_largo_plazo(){
	sem_init(&sem_grado_multiprogramacion,0,get_grado_multiprogramacion());
	init_list_mutex();
	pthread_create(&pcb_new,NULL,(void *)agregar_pcb_a_new,NULL);
	pthread_detach(pcb_new);
}*/

void iniciar_planificador_largo_plazo(){
	pthread_t hilo_new;
	pthread_create(&hilo_new, NULL, (void *)estado_new, NULL);
	pthread_detach(hilo_new);
}


void terminar_programa()
{
	log_destroy(logger);
	config_destroy(config);
	list_mutex_destroy(pcb_new_list);
	list_mutex_destroy(pcb_ready_list);
	list_mutex_destroy(pcb_block_list);
	sem_destroy(&sem_estado_exec);
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

void estado_new(){
	while(1)
	{
		sem_wait(&sem_estado_new);
		if(!list_mutex_is_empty(pcb_new_list)){
			pcb_t* pcb_para_listo = list_pop(pcb_new_list);
			pcb_para_listo->estado = PCB_NEW;
			log_info(logger, "El proceso: %d llego a estado new", pcb_para_listo->pid);
			list_push(pcb_ready_list,pcb_para_listo);
			sem_post(&admitir_pcb);
			//enviar_proceso_a_ejecutar(pcb_a_ejecutar);
		}
		sem_post(&sem_estado_new);
	}
}

void estado_ready() {
	while(1){
		sem_wait(&admitir_pcb);
		//sem_wait(&sem_grado_multiprogramacion);
		pcb_t* pcb_a_ejecutar;

		printf("estado ready \n");

		if (!list_is_empty(pcb_new_list->lista)){
			printf("if 1\n");
			pcb_t* pcb_nuevo = list_pop(pcb_new_list);
			list_push(pcb_ready_list, pcb_nuevo);
			pcb_nuevo->tiempo_espera_en_ready = temporal_create();
		}

		if(strcmp(algoritmo_planificacion, "FIFO") == 0){
			printf("if 2\n");
			pcb_a_ejecutar = list_pop(pcb_ready_list);
			temporal_destroy(pcb_a_ejecutar->tiempo_espera_en_ready);
			enviar_proceso_a_ejecutar(pcb_a_ejecutar);
		}

		else if (strcmp(algoritmo_planificacion, "HRRN") == 0){
			printf("if 3\n");
			pthread_mutex_lock(&(pcb_ready_list->mutex));
			list_sort(pcb_ready_list->lista , mayor_ratio);
			pthread_mutex_unlock(&(pcb_ready_list->mutex));

			//pcb_a_ejecutar = list_pop(pcb_ready_list);

			//temporal_destroy(pcb_a_ejecutar->tiempo_espera_en_ready);

			sem_post(&sem_estado_exec);
			//enviar_proceso_a_ejecutar(pcb_a_ejecutar);
		}
	}
}


void enviar_proceso_a_ejecutar(pcb_t* pcb_a_ejecutar){


/*
 *
 * 		wait(sem_exec == 0)
 *
 *
 * 		t_temporal* tiempo_en_ejecucion = temporal_create(); //NO MODIFICAR (calculo para HRRN)
 *
 * 		serializar_contexto()
 *
 * 		recibir_contexto()
 *
 *      cosas
 *
 *		// En base al tiempo que tardo en ejecutar el proceso, se hace el calculo de la estimación de su proxima rafaga
 *		pcb_a_ejecutar->estimado_proxima_rafaga =  //NO MODIFICAR (calculo para HRRN)
 *			hrrn_alfa * temporal_gettime(tiempo_en_ejecucion)
 *			+ (1-hrrn_alfa) * pcb_a_ejecutar->estimado_proxima_rafaga;
 *
 *		temporal_destroy(tiempo_en_ejecucion);  //NO MODIFICAR (calculo para HRRN)

 *      signal(sem_exec) (reduce el semasforo a 0)
 *
 *
 */


}


void estado_exec(){
	while(1)
	{
		sem_wait(&sem_estado_exec);
		printf("estado exec \n");
		if(!list_mutex_is_empty(pcb_ready_list)){
			pcb_t* pcb_a_ejecutar = list_pop(pcb_ready_list);
			pcb_a_ejecutar->estado = PCB_EXEC;
			log_info(logger, "El proceso: %d llego a estado exec", pcb_a_ejecutar->pid);
			//enviar_proceso_a_ejecutar(pcb_a_ejecutar);
		}
		sem_post(&sem_estado_exec);
	}
}


void estado_block(){
	while(1){

	}
}
int get_grado_multiprogramacion(){
	return config_get_int_value(config,"GRADO_MAX_MULTIPROGRAMACION");
}
float calcular_ratio(pcb_t* pcb_actual){
	float ratio = (
			temporal_gettime(pcb_actual->tiempo_espera_en_ready)
			+ pcb_actual->estimado_proxima_rafaga
			)
			/pcb_actual->estimado_proxima_rafaga;
	return ratio;
}

bool mayor_ratio(void* proceso_1, void* proceso_2){
	float ratio_1 = calcular_ratio((pcb_t*)proceso_1);
	float ratio_2 = calcular_ratio((pcb_t*)proceso_2);

	return ratio_1 > ratio_2;
}
