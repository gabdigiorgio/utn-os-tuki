/*
 * long_term_planification.h
 *
 *  Created on: May 4, 2023
 *      Author: utnso
 */

#ifndef LONG_TERM_PLANIFICATION_H_
#define LONG_TERM_PLANIFICATION_H_

#include "general_planification_utils.h"
#include "semaphore.h"

void estado_new();
void agregar_pcb_a_new(t_list* instrucciones);
pcb_t *crear_proceso(t_list* instrucciones);
void iniciar_planificador_largo_plazo();


#endif /* LONG_TERM_PLANIFICATION_H_ */
