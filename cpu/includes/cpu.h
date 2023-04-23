/*
 * memoria.h
 *
 *  Created on: Apr 3, 2023
 *      Author: utnso
 */

#ifndef CPU_H_
#define CPU_H_

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
#include "../../shared/includes/tad.h"

void iterator(char* value);
void terminar_programa();

// ___ CONFIG VARIABLES ____

t_log* logger;

int server_connection;

int memoria_connection;



// _____________________

// ___ CONFIG VARIABLES ____
char* memoria_ip;
char* memoria_port;

char* server_port;

t_config* config;

// _____________________


#endif /* CPU_H_ */
