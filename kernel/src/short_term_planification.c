/*
 * short_term_planification.c
 *
 *  Created on: May 24, 2023
 *      Author: utnso
 */

#include "../includes/short_term_planification.h"

void iniciar_planificador_corto_plazo()
{

	pthread_t hilo_ready;
	pthread_t hilo_block;
	pthread_t hilo_exec;
	pthread_create(&hilo_ready, NULL, (void*) estado_ready, NULL);
	pthread_create(&hilo_block, NULL, (void*) estado_block, NULL);
	pthread_create(&hilo_exec, NULL, (void*) estado_exec, NULL);
	pthread_detach(hilo_ready);
	pthread_detach(hilo_block);
	pthread_detach(hilo_exec);
}

void estado_ready()
{
	while (1)
	{
		sem_wait(&sem_estado_ready); // espera a que haya algun proceso en la lista de ready

		char* lista_pids = armar_lista_pids(pcb_ready_list->lista);

		log_info(logger, lista_pids,algoritmo_planificacion);

		free(lista_pids);

		sem_wait(&sem_exec_libre); // espera a que exec este libre para planificar

		if (strcmp(algoritmo_planificacion, "FIFO") == 0)
		{
			// Comportamiento por defecto
			// No se tiene que hacer nada
		}
		else if (strcmp(algoritmo_planificacion, "HRRN") == 0)
		{
			pthread_mutex_lock(&(pcb_ready_list->mutex));
			list_sort(pcb_ready_list->lista, mayor_ratio);
			pthread_mutex_unlock(&(pcb_ready_list->mutex));
		}

		sem_post(&sem_estado_exec);
	}
}

void estado_exec()
{
	while (1)
	{
		sem_wait(&sem_estado_exec); // espera que ready le diga que ya hay proceso para ejecutar

		pcb_t *pcb_a_ejecutar = list_pop(pcb_ready_list);

		log_info(logger, "PID: %d - Estado Anterior: PCB_READY - Estado Actual: PCB_EXEC", pcb_a_ejecutar->pid);

		pcb_a_ejecutar->estado = PCB_EXEC;

		temporal_destroy(pcb_a_ejecutar->tiempo_espera_en_ready);

		t_temporal *tiempo_en_ejecucion = temporal_create(); // Empieza el temporizador de cuanto tarda en ejecutar el proceso

		contexto_estado_t resultado = enviar_contexto(pcb_a_ejecutar);

		// En base al tiempo que tardo en ejecutar el proceso, se hace el calculo de la estimación de su proxima rafaga
		if(resultado != EXIT) pcb_a_ejecutar->estimado_proxima_rafaga = (hrrn_alfa * temporal_gettime(tiempo_en_ejecucion) + (1 - hrrn_alfa) * pcb_a_ejecutar->estimado_proxima_rafaga);

		temporal_destroy(tiempo_en_ejecucion);

		sem_post(&sem_exec_libre); // le avisa a ready que exec ya esta libre
	}
}

void estado_block()
{
	while (1)
	{
		sem_wait(&sem_estado_block);

		pcb_t *pcb_bloqueado = list_pop(pcb_block_list);

		pcb_bloqueado->estado = PCB_BLOCK;

		log_info(logger, "El proceso: %d llego a estado block", pcb_bloqueado->pid);

		t_recurso *recurso_bloqueante = buscar_recurso(lista_recursos, pcb_bloqueado->recurso_bloqueante);

		list_push(recurso_bloqueante->cola_bloqueados, pcb_bloqueado);

	}
}

long double calcular_ratio(pcb_t *pcb_actual)
{
	long double ratio = (((long double) temporal_gettime(pcb_actual->tiempo_espera_en_ready) + (long double) pcb_actual->estimado_proxima_rafaga) / (long double) pcb_actual->estimado_proxima_rafaga);

	return ratio;
}

bool mayor_ratio(void *proceso_1, void *proceso_2)
{
	long double ratio_1 = calcular_ratio((pcb_t*) proceso_1);
	long double ratio_2 = calcular_ratio((pcb_t*) proceso_2);

	return ratio_1 > ratio_2;
}

void io_block(void *args)
{
	t_io_block_args *arguments = (t_io_block_args*) args;

	log_info(logger,"PID: %d - Ejecuta IO: %d",arguments->pcb->pid, arguments->block_time);
	log_info(logger,"PID: %d - Bloqueado por: IO",arguments->pcb->pid);

	arguments->pcb->estado = PCB_BLOCK;

	sleep(arguments->block_time);

	list_push(pcb_ready_list, arguments->pcb);
	arguments->pcb->estado = PCB_READY;
	arguments->pcb->tiempo_espera_en_ready = temporal_create();

	sem_post(&sem_estado_ready);
	free(args);
}

void file_system_read_write_block(t_read_write_block_args* args)
{
	t_read_write_block_args *arguments = args;

	log_info(logger,"PID: %d - Bloqueado por: Read/Write",arguments->pcb->pid);

	arguments->pcb->estado = PCB_BLOCK;

	sem_wait(&sem_compactacion);

	editar_archivo(arguments->contexto,arguments->pcb);

	list_push(pcb_ready_list, arguments->pcb);
	arguments->pcb->estado = PCB_READY;
	arguments->pcb->tiempo_espera_en_ready = temporal_create();
	log_info(logger,"PID: %d - Salio del Bloqueo por: Read/Write",arguments->pcb->pid);

	sem_post(&sem_compactacion);
	sem_post(&sem_estado_ready);

	free(args);
}

void file_system_truncate_block(t_read_write_block_args* args)
{
	t_read_write_block_args *arguments = args;

	log_info(logger,"PID: %d - Bloqueado por: Truncate",arguments->pcb->pid);

	arguments->pcb->estado = PCB_BLOCK;

	manejar_archivo(arguments->contexto,arguments->pcb);

	list_push(pcb_ready_list, arguments->pcb);
	arguments->pcb->estado = PCB_READY;
	arguments->pcb->tiempo_espera_en_ready = temporal_create();
	log_info(logger,"PID: %d - Salio del Bloqueo por: Truncate",arguments->pcb->pid);

	sem_post(&sem_estado_ready);

	free(args);
}
