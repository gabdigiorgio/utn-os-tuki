#ifndef UTILS_H_
#define UTILS_H_

#include<stdio.h>
#include<stdlib.h>
#include<commons/log.h>
#include<commons/string.h>
#include<commons/config.h>
#include<readline/readline.h>
#include "../../shared/includes/tad.h"

t_log* iniciar_logger(void);
t_config* iniciar_config(char * path_config);
void copiar_instruccion_mem(t_instruc_mem* instruccion, t_contexto* contexto);
t_instruc_mem* inicializar_instruc_mem();
#endif /* CLIENT_H_ */
