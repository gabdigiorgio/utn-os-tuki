#ifndef CALCULOS_H_
#define CALCULOS_H_

#include<stdio.h>
#include<commons/log.h>

#include "../../shared/includes/tad.h"

uint32_t calcular_tam_instrucciones(t_list* lista);
uint32_t calcular_tam_tabla_segmentos(tabla_segmentos_t * tabla_segmento);
uint32_t calcular_tam_contexto(t_contexto* contexto);
uint32_t calcular_tam_registros(t_registros* registros);
uint32_t calcular_tam_instruc_mem(t_instruc_mem* instruccion);

#endif /* CALCULOS_H_ */
