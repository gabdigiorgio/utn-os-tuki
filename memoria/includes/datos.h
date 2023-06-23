#ifndef DATOS_H_
#define DATOS_H_

#include<stdio.h>
#include<commons/log.h>
#include<commons/collections/list.h>

#include "../../shared/includes/tad.h"

void copiar_tabla_segmentos(void *stream, t_list *lista_tablas);
void copiar_instruccion_memoria(void *stream, t_instruc_mem *instruccion);
void copiar_instruccion_mem(t_instruc_mem* instruccion, t_contexto* contexto);
t_instruc_mem* inicializar_instruc_mem();

#endif /* CLIENT_UTILS_H_ */
