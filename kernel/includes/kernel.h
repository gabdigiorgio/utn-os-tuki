#ifndef KERNEL_H_
#define KERNEL_H_

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <commons/log.h>
#include <commons/string.h>
#include <pthread.h>

#include "server_utils.h"
#include "client_utils.h"
#include "console_threads.h"
#include "utils.h"
#include "initial_setup.h"
#include "../../shared/includes/tad.h"

#define CANTIDAD_DE_THREADS 4

void iterator(char* value);
void terminar_programa();

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
+
typedef struct{
	archivo_abierto_t* archivos_abiertos;
}tabla_archivos_abiertos_t;

typedef struct {
	uint32_t pid;
	char* estado; // tipo de estado, puede ser un enum
	uint32_t program_counter;
	uint32_t estimado_proxima_rafaga; // se saca inicialmente del config
	uint32_t tiempo_llegada_ready; // se hace con timer, ver timestamp
	//registros_cpu_t registros_cpu; // crear struct de registros de cpu
	tabla_segmentos_t tabla_segmento; // nada
	t_list* instrucciones; // lista recibida de consola
	tabla_archivos_abiertos_t tabla_archivos; // nada
}pcb_t;

// ___ CONFIG VARIABLES ____

t_log* logger;

int server_connection;

int memoria_connection;
int file_system_connection;
int cpu_connection;

int cant_threads_activos;

// _____________________

// ___ CONFIG VARIABLES ____
char* memoria_ip;
char* memoria_port;
char* file_system_ip;
char* file_system_port;
char* cpu_ip;
char* cpu_port;

char* server_port;

t_config* config;

//


#endif /* KERNEL_H_ */
