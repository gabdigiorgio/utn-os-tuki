/*
 * segmentacion.c
 *
 *  Created on: Jun 6, 2023
 *      Author: utnso
 */

#include "../includes/segmentacion.h"


segmento_t crear_segmento(int id, int direccion_base, int tamanio, int pid) {
	segmento_t *segmento = malloc(sizeof(segmento_t));
	segmento->ids = id;
	segmento->direccion_base = direccion_base;
	segmento->tamanio = tamanio;
	tabla_segmentos_t *tabla = buscar_tabla(lista_de_tablas,pid);
	list_add(tabla->segmentos,segmento);
	return segmento;
}


tabla_segmentos_t* buscar_tabla(t_list *lista, int pid)
{
	for (int i = 0; i < list_size(lista); i++)
	{
		tabla_segmentos_t *tabla = (tabla_segmentos_t*) list_get(lista, i);
		if (tabla->pid - pid == 0)
		{
			return tabla;
		}
	}
	return NULL;
}
