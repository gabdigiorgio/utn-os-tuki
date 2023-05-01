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
	#include "../../shared/includes/tad.h"
	#include "semaphore.h"

// ____________________________________

// -------------------------------
// 		   STRUCTS
// -------------------------------



	typedef struct{
		 uint32_t ids;
		 uint32_t direccion_base;
		 uint32_t tamanio;
	}segmento_t;


	typedef struct{
		 segmento_t* segmento;
	}tabla_segmentos_t;


	typedef struct{
		 char* instrucciones;
	}lista_instucciones_t;


	typedef struct{
		uint32_t id_archivo;
		uint32_t posicion_puntero;
	}archivo_abierto_t;


	typedef struct{
		archivo_abierto_t* archivos_abiertos;
	}tabla_archivos_abiertos_t;


	typedef enum {
		PCB_NEW,
		PCB_READY,
		PCB_EXEC,
		PCB_BLOCK,
		PCB_EXIT
	} pcb_estado_t;


	typedef struct {
		uint32_t pid;
		pcb_estado_t estado; // tipo de estado, puede ser un enum
		uint32_t program_counter;
		uint32_t estimado_proxima_rafaga; // se saca inicialmente del config
		t_temporal* tiempo_espera_en_ready; // se hace con timer, ver timestamp
		//registros_cpu_t registros_cpu; // crear struct de registros de cpu
		tabla_segmentos_t tabla_segmento; // nada
		t_list* instrucciones; // lista recibida de consola
		tabla_archivos_abiertos_t tabla_archivos; // nada
	}pcb_t;


// ____________________________________





// -------------------------------
// 		  GLOBAL VARIABLES
// -------------------------------

	t_log* logger;

	int server_connection;

	int memoria_connection;
	int file_system_connection;
	int cpu_connection;

	int cant_threads_activos;

	bool recibido = 0;

	t_lista_mutex *pcb_ready_list;
	t_lista_mutex *pcb_new_list;
	t_lista_mutex *pcb_block_list;

	int pid;

	// Semaforos

	sem_t sem_estado_exec;
	sem_t sem_estado_new;
	sem_t sem_estado_ready;
	sem_t sem_exec_libre;

	sem_t sem_pid_aumento;

	sem_t sem_grado_multiprogramacion;



	// Largo Plazo

//	sem_t mutex_list_new;
//	sem_t mutex_list_exit;
//	sem_t sem_list_exit;



	// Planificador a Largo Plazo
	//pthread_t pcb_new;

// ____________________________________



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
	void iniciar_planificador_largo_plazo();
	void iniciar_planificador_corto_plazo();
	void estado_ready();
	void enviar_proceso_a_ejecutar(pcb_t* pcb_a_ejecutar);
	void estado_block();
	void estado_new();
	void estado_exec();
	void terminar_programa();
	long double calcular_ratio(pcb_t* pcb_actual);
	bool mayor_ratio(void* proceso_1, void* proceso_2);
	void enviar_contexto(t_contexto* contexto);
	t_contexto* obtener_contexto_pcb(pcb_t* pcb);
	void iniciar_semaforos();
	pcb_t *crear_proceso(t_list* instrucciones);
	void agregar_pcb_a_new(t_list* instrucciones);

// ____________________________________________________________




#endif /* KERNEL_H_ */
