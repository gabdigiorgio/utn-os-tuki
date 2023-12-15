#ifndef MONITOR_MEM_H_
#define MONITOR_MEM_H_

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <commons/config.h>
#include "server_utils.h"
#include "utils.h"
#include "initial_setup.h"

t_config* config;
t_log* logger;
char* server_port;
int tam_linea;

void conexion_memoria(int server_connection);

#endif /* MONITOR_MEM_H_ */
