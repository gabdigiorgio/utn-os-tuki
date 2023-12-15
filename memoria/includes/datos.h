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
void generar_instruccion_mov(t_instruc_mem* instruccion_nueva,contexto_estado_t instruccion, uint32_t dir_fisica, uint32_t tamanio);
t_instruc_mov* inicializar_instruc_mov();
void copiar_instruccion_mov(void *stream, t_instruc_mov *instruccion);

#endif /* CLIENT_UTILS_H_ */
