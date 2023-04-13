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

// _____________________


#endif /* KERNEL_H_ */
