#ifndef DATOS_H_
#define DATOS_H_

#include<stdio.h>
#include<string.h>

#include "../../shared/includes/tad.h"
#include"utils.h"

void copiar_contexto(void* stream, t_contexto* contexto);
void copiar_instruccion_memoria(void* stream, t_instruc_mem* instruccion);
t_contexto* contexto_create();
void contexto_destroy(t_contexto* contexto);
void instrucciones_destroy(t_instruc* instruccion);
t_instruc_mem* inicializar_instruc_mem();
void copiar_instruccion_mem(t_instruc_mem* instruccion, t_contexto* contexto);
void destroy_instruc_mem(t_instruc_mem* instruccion);

#endif /* CLIENT_UTILS_H_ */
