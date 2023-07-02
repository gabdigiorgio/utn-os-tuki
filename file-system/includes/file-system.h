#ifndef FILE_SYSTEM_H_
#define FILE_SYSTEM_H_

#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include<commons/log.h>
#include<commons/string.h>
#include<commons/config.h>
#include<commons/bitarray.h>
#include<commons/collections/queue.h>
#include<readline/readline.h>
#include<fcntl.h>
#include<sys/mman.h>
#include<unistd.h>

#include "comm_Mem.h"
#include "comm_ThreadKernel.h"
#include "client_utils.h"
#include "server_utils.h"
#include "utils.h"
#include "initial_setup.h"
#include "bitmap_utils.h"
#include "fcb_list.h"

void iterator(char* value);
void generar_fcb();
void terminar_programa();
void asignar_bloques(int id_fcb, int nuevo_tamanio);
void desasignar_bloques(int id_fcb, int nuevo_tamanio);
int obtener_cantidad_de_bloques(int id_fcb);
t_list* obtener_lista_de_bloques(int id_fcb);
void thread_monitor(int connection);

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

// Bitmap
t_bitarray *bitmap;

// _____________________

fcb_t* fcb;
int fcb_id;
char* nombre_archivo;
int tamanio_archivo;
int puntero_directo;
int puntero_indirecto;

int exit_status = 0;
fcb_list_t* lista_global_fcb;
t_list *fcb_list;

//___GLOBAL VARIABLES___

void* memoria_file_system;
int tam_memoria_file_system;
uint32_t  *array_de_bloques;

#endif
