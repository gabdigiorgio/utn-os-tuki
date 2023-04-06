#ifndef KERNEL_H_
#define KERNEL_H_

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <commons/log.h>
#include <commons/string.h>
#include <commons/config.h>
#include <readline/readline.h>

#include "server_utils.h"
#include "client_utils.h"
#include "utils.h"
#include "initial_setup.h"

void iterator(char* value);
void terminar_programa(int conexion, t_log* logger, t_config* config);



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
