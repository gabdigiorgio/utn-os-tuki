#ifndef MEMORIA_H_
#define MEMORIA_H_

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <commons/log.h>
#include <stdbool.h>
#include <pthread.h>

#include "server_utils.h"
#include "utils.h"
#include "initial_setup.h"
#include "segmentacion.h"
#include "comm_threadCpu.h"
#include "comm_threadFile.h"
#include "comm_threadKernel.h"

#define CANTIDAD_DE_THREADS 3

typedef struct
{
	int num_socket;
	int id_cliente;
} t_conexion;

void thread_main(t_conexion *conexion);


// ___ CONFIG VARIABLES ____

t_log* logger;
int server_connection;
pthread_t tid[3];
char* server_port;
int tam_memoria;
int tam_segmento_0;
int cant_segmentos;
int retardo_memoria;
int retardo_compactacion;
char* algoritmo_asignacion;
int cpu_connection;
int kernel_connection;
int fileSystem_connection;
t_config* config;

// _____________________

// _____ GLOBAL VARIABLES

void * memoria;

t_list *lista_de_tablas;

t_list *lista_de_huecos_libres;

bool cpu_conectada;
bool kernel_conectado;
bool fileSystem_conectado;

int exit_status = 0;


// _____________________


#endif /* MEMORIA_H_ */
