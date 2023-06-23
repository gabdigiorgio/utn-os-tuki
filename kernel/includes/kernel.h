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
	#include "comm_Mem.h"
	#include "utils.h"
	#include "initial_setup.h"
	#include "general_planification_utils.h"
	#include "long_term_planification.h"
	#include "short_term_planification.h"
	#include "../../shared/includes/tad.h"


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

	t_lista_mutex * lista_recursos;
	t_lista_mutex * lista_tabla_segmentos;

	t_list * tabla_global_archivos_abiertos;

	// Semaforos

	sem_t sem_estado_exec;
	sem_t sem_estado_new;
	sem_t sem_estado_ready;
	sem_t sem_estado_block;
	sem_t sem_exec_libre;
	sem_t sem_estado_exit;
	sem_t sem_pid_aumento;
	sem_t sem_grado_multi;

	//sem_t sem_recursos[N];

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

	int grado_max_multiprogramacion;

	char** lista_nombre_recursos;
	char** lista_instancias_recursos;

	t_config* config;

// __________________________________


// -------------------------------
// 			FUNCIONES
// -------------------------------

	void iterator(char* value);
	void iniciar_pcb_lists();
	void iniciar_semaforos();
	void terminar_programa();
	void iniciar_lista_recursos();
	void iniciar_tabla_segmentos();

// ____________________________________________________________




#endif /* KERNEL_H_ */
