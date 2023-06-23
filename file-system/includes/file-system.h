#ifndef FILE_SYSTEM_H_
#define FILE_SYSTEM_H_

#include<stdio.h>
#include<stdlib.h>
#include<commons/log.h>
#include<commons/string.h>
#include<commons/config.h>
#include<readline/readline.h>

#include "comm_Mem.h"
#include "comm_ThreadKernel.h"
#include "client_utils.h"
#include "server_utils.h"
#include "utils.h"
#include "initial_setup.h"

void iterator(char* value);
void generar_fcb();
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

char* path_superbloque;
char* path_bitmap;
char* path_bloques;
char* path_fcb_folder;

int retardo_acceso_bloque;

// Variables de SuperBloque
int tamanio_de_bloque;
int cantidad_de_bloques;

t_config* config;
t_config* superbloque_config;

// _____________________

fcb_t* fcb;
char* nombre_archivo;
int tamanio_archivo;
int puntero_directo;
int puntero_indirecto;

t_list *fcb_list;


#endif
