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
		sem_wait(&sem_grado_multi); //consultar por un tema de deadlock
		sem_wait(&sem_estado_new);
		pcb_t* pcb_para_listo = list_pop(pcb_new_list);
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
	log_info(logger, "El proceso: %d llego a estado new", proceso->pid);
	sem_post(&sem_estado_new);
}

pcb_t *crear_proceso(t_list* instrucciones){
	pcb_t *proceso = malloc(sizeof(pcb_t));
	sem_wait(&sem_pid_aumento);
	proceso->pid = pid++;
	sem_post(&sem_pid_aumento);
	proceso->estimado_proxima_rafaga = estimacion_inicial;
	proceso->instrucciones = instrucciones;
	proceso->registros_cpu.ip=0;
	strcpy(proceso->registros_cpu.ax,  "0");
	strcpy(proceso->registros_cpu.bx,  "0");
	strcpy(proceso->registros_cpu.cx,  "0");
	strcpy(proceso->registros_cpu.dx,  "0");
	strcpy(proceso->registros_cpu.eax,  "0");
	strcpy(proceso->registros_cpu.ebx,  "0");
	strcpy(proceso->registros_cpu.ecx,  "0");
	strcpy(proceso->registros_cpu.edx,  "0");
	strcpy(proceso->registros_cpu.rax,  "0");
	strcpy(proceso->registros_cpu.rbx,  "0");
	strcpy(proceso->registros_cpu.rcx,  "0");
	strcpy(proceso->registros_cpu.rdx,  "0");
	proceso->estado = PCB_NEW;
	return proceso;
}

void estado_exit(){
	while(1){
		sem_wait(&sem_estado_exit);
		pcb_t* proceso = list_pop(pcb_exit_list);
		proceso->estado=PCB_EXIT;
		log_info(logger, "El proceso: %d ingreso a exit", proceso->pid);
		log_info(logger, "El proceso: %d finalizo definitivamente", proceso->pid);
		//hace el free de todo lo que tiene adentro el pcb
		free(proceso);
		sem_post(&sem_grado_multi);
		//comunicar que termino a la consola
	}
}
void iniciar_planificador_largo_plazo(){
	pthread_t hilo_new;
	pthread_t hilo_exit;
	pthread_create(&hilo_new, NULL, (void *)estado_new, NULL);
	pthread_create(&hilo_exit,NULL, (void *)estado_exit,NULL);
	pthread_detach(hilo_new);
	pthread_detach(hilo_exit);
}
