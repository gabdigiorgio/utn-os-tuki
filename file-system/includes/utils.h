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

t_log* iniciar_logger(void);
fcb_t* iniciar_fcb(char* path_fcb);
t_config* iniciar_config(char * path_config);
t_instruc_file* inicializar_instruc_file();
void copiar_instruccion_file(t_instruc_file* instruccion, t_contexto* contexto);
t_instruc_mem* inicializar_instruc_mem();
void copiar_instruccion_mem(t_instruc_mem* instruccion, t_instruc_file* contexto);
fcb_t* inicializar_fcb();

#endif /* CLIENT_H_ */
