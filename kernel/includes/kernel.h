#ifndef KERNEL_H_
#define KERNEL_H_

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <commons/log.h>
#include <pthread.h>

#include "server_utils.h"
#include "client_utils.h"
#include "console_threads.h"
#include "utils.h"
#include "initial_setup.h"

void iterator(char* value);
void terminar_programa();



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
