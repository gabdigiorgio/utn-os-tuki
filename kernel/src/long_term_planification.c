/*
 * long_term_planification.c
 *
 *  Created on: May 4, 2023
 *      Author: utnso
 */
#include "../includes/long_term_planification.h"

void estado_new(){
	while(1)
	{
		sem_wait(&sem_estado_new);
		pcb_t* pcb_para_listo = list_pop(pcb_new_list);
		pcb_para_listo->estado = PCB_NEW;
		log_info(logger, "El proceso: %d llego a estado new", pcb_para_listo->pid);
		list_push(pcb_ready_list,pcb_para_listo);
		pcb_para_listo->tiempo_espera_en_ready = temporal_create();
		pcb_para_listo->estado = PCB_READY;
		log_info(logger, "El proceso: %d se agrego a la lista de ready", pcb_para_listo->pid);
		sem_post(&sem_estado_ready);
	}
}

void agregar_pcb_a_new(t_list* instrucciones){
	//uint32_t largo = list_mutex_size(pcb_new_list);
	pcb_t *proceso = crear_proceso(instrucciones);
	list_push(pcb_new_list,proceso);
	sem_post(&sem_estado_new);
}

pcb_t *crear_proceso(t_list* instrucciones){
	pcb_t *proceso = malloc(sizeof(pcb_t));
	sem_wait(&sem_pid_aumento);
	proceso->pid = pid++;
	sem_post(&sem_pid_aumento);
	proceso->estimado_proxima_rafaga = estimacion_inicial;
	proceso->instrucciones = instrucciones;
	//Desde aqui se asignarian los tiempos para manejar los algoritmos de planificacion asignando los que inician en 0 y el estado como new
	return proceso;
}

void iniciar_planificador_largo_plazo(){
	pthread_t hilo_new;
	pthread_create(&hilo_new, NULL, (void *)estado_new, NULL);
	pthread_detach(hilo_new);
}
