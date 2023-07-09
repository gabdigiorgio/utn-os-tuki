#include "../../../includes/fcb_list.h"

t_list* obtener_lista_total_de_bloques(int id_fcb){
	return obtener_lista_de_bloques(id_fcb,0,0);
}

t_list* obtener_lista_de_bloques(int id_fcb,int offset_inicial, int size)
{
	t_list *lista_de_bloques = list_create();
	int tamanio_archivo = valor_fcb(id_fcb, TAMANIO_ARCHIVO);

	if(size + offset_inicial > tamanio_archivo) {
		log_error(logger,"El tamanio a leer es mayor al tamanio del archivo seleccionado");
		return lista_de_bloques;
	}

	//int cant_bloques_fcb = size == 0 ? obtener_cantidad_de_bloques(id_fcb) : ceil((double)size/tamanio_de_bloque);
	int cant_bloques_fcb = obtener_cantidad_de_bloques(id_fcb);
	if(size == 0) size = cant_bloques_fcb * tamanio_de_bloque;
	int offset_fcb = offset_inicial;

	if (offset_inicial < tamanio_de_bloque){
		offset_fcb_t *bloque_directo = malloc(sizeof(offset_fcb_t));
		bloque_directo->id_bloque = valor_fcb(id_fcb, PUNTERO_DIRECTO);
		bloque_directo->offset = bloque_directo->id_bloque * tamanio_de_bloque;
		cant_bloques_fcb--;
		offset_fcb += tamanio_de_bloque - offset_inicial;

		list_add(lista_de_bloques, bloque_directo);
	}

	if (cant_bloques_fcb > 1)
	{
		leer_bloques_indirectos(id_fcb,lista_de_bloques,offset_fcb,size + offset_inicial);
	}

	return lista_de_bloques;
}