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
		sem_wait(&sem_grado_multi);
		sem_wait(&sem_estado_new);
		pcb_t* pcb_para_listo = list_pop(pcb_new_list);
		list_push(pcb_ready_list,pcb_para_listo);
		pcb_para_listo->tiempo_espera_en_ready = temporal_create();
		pcb_para_listo->estado = PCB_READY;
		log_info(logger, "PID: %d - Estado Anterior: PCB_NEW - Estado Actual: PCB_READY", pcb_para_listo->pid);
		sem_post(&sem_estado_ready);
	}
}

void agregar_pcb_a_new(t_list* instrucciones, uint32_t socket){
	pcb_t *proceso = crear_proceso(instrucciones, socket);
	list_push(pcb_new_list,proceso);
	log_info(logger, "Se crea el proceso: %d en NEW", proceso->pid);
	sem_post(&sem_estado_new);
}

pcb_t *crear_proceso(t_list* instrucciones, uint32_t socket){
	pcb_t *proceso = malloc(sizeof(pcb_t));
	sem_wait(&sem_pid_aumento);
	proceso->pid = ++pid;
	sem_post(&sem_pid_aumento);
	proceso->consola = socket;
	proceso->estimado_proxima_rafaga = estimacion_inicial;
	proceso->instrucciones = list_create();
	copiar_lista_instrucciones(proceso->instrucciones,instrucciones);
	proceso->registros_cpu = inicializar_registros();
	proceso->estado = PCB_NEW;
	proceso->recurso_bloqueante = malloc(sizeof(char)*2);
	proceso->recursos_asignados = list_create();
	proceso->tabla_segmento = solicitar_segmento_0(proceso->pid);
	memcpy(proceso->recurso_bloqueante,"0",(sizeof(char)*2));
	return proceso;
}

void estado_exit(){
	while(1){
		uint8_t resultado = 1;
		sem_wait(&sem_estado_exit);
		pcb_t* proceso = list_pop(pcb_exit_list);
		proceso->estado=PCB_EXIT;
		log_info(logger, "Finaliza el proceso %d - Motivo: SUCCESS", proceso->pid);
		send(proceso->consola, &resultado, sizeof(uint8_t), NULL);
		//hace el free de todo lo que tiene adentro el pcb
		destroy_proceso(proceso);
		sem_post(&sem_grado_multi);
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



