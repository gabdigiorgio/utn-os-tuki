#ifndef CONSOLA_H_
#define CONSOLA_H_

#include<stdio.h>
#include<stdlib.h>
#include<commons/log.h>
#include<commons/string.h>
#include<commons/config.h>
#include<commons/collections/queue.h>
#include<readline/readline.h>

#include "client_utils.h"
#include "utils.h"
#include "initial_setup.h"


void leer_consola(t_log*);
void paquete(int);
void terminar_programa();
int leer_pseudocodigo(char* path);



// ___ CONFIG VARIABLES ____

t_log* logger;

int kernel_connection;


// _____________________

// ___ CONFIG VARIABLES ____
char* kernel_ip;
char* kernel_port;

t_config* config;

// _____________________

// ___ INSTRUCCIONES ____

typedef struct
{
	int nro;
	char* instruct;
    char* param1;
    char* param2;
    char* param3;
} t_instruc;

t_queue* instruc_queue;

// _____________________

#endif
