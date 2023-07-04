#ifndef UTILS_H_
#define UTILS_H_

#include "../../shared/includes/tad.h"
#include<stdio.h>
#include<stdlib.h>
#include<stdbool.h>
#include<commons/log.h>
#include<commons/string.h>
#include<commons/config.h>
#include<readline/readline.h>
#include <commons/collections/list.h>
#include <pthread.h>

#include "fcb_list.h"
#include "handler_file.h"

extern void* memoria_file_system;
extern int tam_memoria_file_system;
extern int tamanio_de_bloque;
extern t_log* logger;

t_log* iniciar_logger(void);
fcb_t* iniciar_fcb(char* path_fcb);
t_config* iniciar_config(char * path_config);
t_instruc_file* inicializar_instruc_file();
t_instruc_mem* inicializar_instruc_mem();
void copiar_instruccion_mem(t_instruc_mem* instruccion, t_instruc_file* contexto);
fcb_t* inicializar_fcb();
void inicializar_datos_memoria();
void* list_pop(t_list* list);
t_list* armar_lista_offsets(int id_fcb, int tam_a_leer, int p_seek);

#endif /* CLIENT_H_ */
