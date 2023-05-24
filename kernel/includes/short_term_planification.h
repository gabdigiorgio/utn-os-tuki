/*
 * short_term_planification.h
 *
 *  Created on: May 24, 2023
 *      Author: utnso
 */

#ifndef SHORT_TERM_PLANIFICATION_H_
#define SHORT_TERM_PLANIFICATION_H_

#include "general_planification_utils.h"
#include "semaphore.h"
#include<stdbool.h>

void iniciar_planificador_corto_plazo();
void estado_ready();
void estado_exec();
void estado_block();
long double calcular_ratio(pcb_t* pcb_actual);
bool mayor_ratio(void* proceso_1, void* proceso_2);

#endif /* SHORT_TERM_PLANIFICATION_H_ */
