/*
 * general_planification_utils.h
 *
 *  Created on: May 4, 2023
 *      Author: utnso
 */

#ifndef GENERAL_PLANIFICATION_UTILS_H_
#define GENERAL_PLANIFICATION_UTILS_H_

	#include <stdio.h>
	#include <stdlib.h>
	#include <string.h>
	#include <commons/log.h>
	#include <commons/string.h>
	#include <pthread.h>
	#include <semaphore.h>
	#include <commons/collections/list.h>
	#include <commons/temporal.h>


	#include "server_utils.h"
	#include "client_utils.h"
	#include "console_threads.h"
	#include "utils.h"
	#include "initial_setup.h"
	#include "../../shared/includes/tad.h"
	#include "semaphore.h"

	extern t_lista_mutex *pcb_ready_list;
	extern t_lista_mutex *pcb_new_list;
	extern t_lista_mutex *pcb_block_list;
	extern t_lista_mutex *pcb_exit_list;

	extern int pid;

	// Semaforos

	extern sem_t sem_estado_exec;
	extern sem_t sem_estado_new;
	extern sem_t sem_estado_ready;
	extern sem_t sem_exec_libre;
	extern sem_t sem_estado_exit;
	extern sem_t sem_pid_aumento;
	extern sem_t sem_grado_multi;

#endif /* GENERAL_PLANIFICATION_UTILS_H_ */
