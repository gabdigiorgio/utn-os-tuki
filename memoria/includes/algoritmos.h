/*
 * algoritmos.h
 *
 *  Created on: Jun 11, 2023
 *      Author: utnso
 */

#ifndef ALGORITMOS_H_
#define ALGORITMOS_H_

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <commons/log.h>

#include "../../shared/includes/tad.h"
#include "segmentacion.h"
#include "utils.h"

extern t_list *lista_de_huecos_libres;
extern int tam_memoria;

int first(uint32_t pid, int id_segmento, int tamanio_segmento);
int best(uint32_t pid, int id_segmento, int tamanio_segmento);
int worst(uint32_t pid, int id_segmento, int tamanio_segmento);


#endif /* ALGORITMOS_H_ */
