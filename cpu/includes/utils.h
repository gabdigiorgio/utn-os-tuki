#ifndef UTILS_H_
#define UTILS_H_

#include<stdio.h>
#include<stdlib.h>
#include<commons/log.h>
#include<commons/string.h>
#include<commons/config.h>
#include<readline/readline.h>

#include "../../shared/includes/tad.h"

extern t_log* logger;

t_log* iniciar_logger(void);
t_config* iniciar_config(char * path_config);
bool sort_list(t_instruc* instruc1, t_instruc* instruc2);
void copiar_string(char* origen, char* destino);
void contexto_destroy(t_contexto* contexto);
void instrucciones_destroy(t_instruc* instruccion);
t_contexto* contexto_create();
t_registros* inicializar_registros();
#endif /* CLIENT_H_ */
