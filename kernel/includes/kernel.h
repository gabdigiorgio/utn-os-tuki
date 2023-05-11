#ifndef KERNEL_H_
#define KERNEL_H_

// -------------------------------
// 		  INCLUDES
// -------------------------------

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
	#include "general_planification_utils.h"
	#include "long_term_planification.h"
	#include "../../shared/includes/tad.h"
	#include "semaphore.h"


// ____________________________________

// -------------------------------
// 		   STRUCTS
// -------------------------------
//
bool recibido = 0;

	t_lista_mutex *pcb_ready_list;
	t_lista_mutex *pcb_new_list;
	t_lista_mutex *pcb_block_list;
	t_lista_mutex *pcb_exit_list;

	int pid;

	// Semaforos

	sem_t sem_estado_exec;
	sem_t sem_estado_new;
	sem_t sem_estado_ready;
	sem_t sem_exec_libre;
	sem_t sem_estado_exit;
	sem_t sem_pid_aumento;

	sem_t sem_grado_multiprogramacion;



	// Largo Plazo

//	sem_t mutex_list_new;
//	sem_t mutex_list_exit;
//	sem_t sem_list_exit;



	// Planificador a Largo Plazo
	//pthread_t pcb_new;

// ____________________________________
//____________________________________
// -------------------------------
// 		  GLOBAL VARIABLES
// -------------------------------

	t_log* logger;

	int server_connection;

	int memoria_connection;
	int file_system_connection;
	int cpu_connection;

	int cant_threads_activos;




// -------------------------------
// 		  CONFIG VARIABLES
// -------------------------------

	char* memoria_ip;
	char* memoria_port;
	char* file_system_ip;
	char* file_system_port;
	char* cpu_ip;
	char* cpu_port;

	char* server_port;

	char* algoritmo_planificacion;
	int estimacion_inicial;
	float hrrn_alfa;

	float grado_max_multiprogramacion;

	t_config* config;

// __________________________________


// -------------------------------
// 			FUNCIONES
// -------------------------------

	void iterator(char* value);
	void iniciar_pcb_lists();
	void iniciar_semaforos();
	void enviar_contexto(t_contexto* contexto);
	t_contexto* obtener_contexto_pcb(pcb_t* pcb);
	void enviar_proceso_a_ejecutar(pcb_t* pcb_a_ejecutar);
	void iniciar_planificador_corto_plazo();
	void estado_ready();
	void estado_exec();
	void estado_block();
	long double calcular_ratio(pcb_t* pcb_actual);
	bool mayor_ratio(void* proceso_1, void* proceso_2);

// ____________________________________________________________




#endif /* KERNEL_H_ */
