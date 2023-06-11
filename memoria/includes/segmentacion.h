/*
 * segmentacion.h
 *
 *  Created on: Jun 6, 2023
 *      Author: utnso
 */

#ifndef SEGMENTACION_H_
#define SEGMENTACION_H_

#include "utils.h"
#include "../../shared/includes/tad.h"

extern t_list *lista_de_tablas;

void crear_segmento(int id, int direccion_base, int tamanio,int pid);
tabla_segmentos_t* buscar_tabla(int pid);

#endif /* SEGMENTACION_H_ */
