#ifndef CONSOLA_H_
#define CONSOLA_H_

#include<stdio.h>
#include<stdlib.h>
#include<commons/log.h>
#include<commons/string.h>
#include<commons/config.h>
#include<readline/readline.h>

#include "client_utils.h"
#include "utils.h"
#include "initial_setup.h"


void leer_consola(t_log*);
void paquete(int);
void terminar_programa();



// ___ CONFIG VARIABLES ____

t_log* logger;

int kernel_connection;


// _____________________

// ___ CONFIG VARIABLES ____
char* kernel_ip;
char* kernel_port;

t_config* config;

// _____________________

#endif