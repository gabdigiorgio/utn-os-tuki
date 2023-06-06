/*
 * segmentacion.c
 *
 *  Created on: Jun 6, 2023
 *      Author: utnso
 */

#include "../includes/segmentacion.h"
segmento_t crear_segmento(int id, int direccion_base, int tamanio) {
	segmento_t *segmento = malloc(sizeof(segmento_t));
	segmento->ids = id;
	segmento->direccion_base = direccion_base;
	segmento->tamanio = tamanio;
	return segmento;
}
