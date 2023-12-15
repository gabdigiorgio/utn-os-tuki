#include "../../includes/algoritmos.h"

// 0: Se puede crear segmento | 1: Out of memory | 2: Se necesita compactación

int first(uint32_t pid, int id_segmento, int tamanio_segmento){
	int estado_first = 	COMPACTION_NEEDED;
	for (int i = 0; i < list_size(lista_de_huecos_libres); i++)
	{
		hueco_libre_t *hueco_libre = (hueco_libre_t*) list_get(lista_de_huecos_libres, i);


		if (hueco_libre->tamanio >= tamanio_segmento)
		{
			uint32_t base_segmento = hueco_libre->direccion_base;
			if(hueco_libre->tamanio - tamanio_segmento == 0){ // si el tamanio del hueco libre es 0, entonces lo elimino
				list_remove_element(lista_de_huecos_libres, hueco_libre);
			}
			else { //Si el tamanio del hueco libre no es 0, entonces modifico su base y su tamanio
				hueco_libre->direccion_base = hueco_libre->direccion_base + tamanio_segmento;
				hueco_libre->tamanio = hueco_libre->tamanio - tamanio_segmento;
			}

			estado_first = SUCCESS_CREATE_SEGMENT;
			crear_segmento(id_segmento,base_segmento, tamanio_segmento, pid);
			log_algoritmos(pid,id_segmento,tamanio_segmento,base_segmento);
			break;
		}

	}

	return estado_first;
}



int best(uint32_t pid, int id_segmento, int tamanio_segmento){
	int indice_hueco_mas_chico;
	int tamanio_menor;
	hueco_libre_t *hueco_libre;

	indice_hueco_mas_chico = -1;
	tamanio_menor = tam_memoria;

	log_info(logger, "BEST");
	for (int i = 0; i < list_size(lista_de_huecos_libres); i++)
	{
		hueco_libre = (hueco_libre_t*) list_get(lista_de_huecos_libres, i);
		 if(hueco_libre->tamanio < tamanio_menor && hueco_libre->tamanio  >= tamanio_segmento){
			 indice_hueco_mas_chico = i;
			 tamanio_menor = hueco_libre->tamanio;
		 }

	}

	if (indice_hueco_mas_chico == -1)
		return COMPACTION_NEEDED;
	else{

		hueco_libre = (hueco_libre_t*) list_get(lista_de_huecos_libres, indice_hueco_mas_chico);

		uint32_t base_segmento = hueco_libre->direccion_base;
		if (hueco_libre->tamanio > tamanio_segmento)
		{
			hueco_libre->direccion_base = hueco_libre->direccion_base + tamanio_segmento;
			hueco_libre->tamanio = hueco_libre->tamanio - tamanio_segmento;

		} else {
			list_remove_element(lista_de_huecos_libres, hueco_libre);
		}

		crear_segmento(id_segmento,base_segmento, tamanio_segmento,pid);
		log_algoritmos(pid,id_segmento,tamanio_segmento,base_segmento);

		return SUCCESS_CREATE_SEGMENT;
	}
}



int worst(uint32_t pid, int id_segmento, int tamanio_segmento){
	int indice_hueco_mas_grande;
	int tamanio_mayor;

	hueco_libre_t *hueco_libre;

	indice_hueco_mas_grande = -1;
	tamanio_mayor = tamanio_segmento;

	for (int i = 0; i < list_size(lista_de_huecos_libres); i++)
	{
		hueco_libre = (hueco_libre_t*) list_get(lista_de_huecos_libres, i);
		 if(hueco_libre->tamanio >= tamanio_mayor){
			 indice_hueco_mas_grande = i;
			 tamanio_mayor = hueco_libre->tamanio;
		 }

	}

	if (indice_hueco_mas_grande == -1 )
		return COMPACTION_NEEDED;
	else{
		hueco_libre = (hueco_libre_t*) list_get(lista_de_huecos_libres, indice_hueco_mas_grande);

		uint32_t base_segmento = hueco_libre->direccion_base;
		if (hueco_libre->tamanio > tamanio_segmento)
		{
			hueco_libre->direccion_base = hueco_libre->direccion_base + tamanio_segmento;
			hueco_libre->tamanio = hueco_libre->tamanio - tamanio_segmento;

		} else {
			list_remove_element(lista_de_huecos_libres, hueco_libre);
		}

		crear_segmento(id_segmento,base_segmento, tamanio_segmento,pid);
		log_algoritmos(pid,id_segmento,tamanio_segmento,base_segmento);

		return SUCCESS_CREATE_SEGMENT;
	}
}

void log_algoritmos(uint32_t pid, int id_segmento, int tamanio_segmento, int base_segmento){
	log_info(logger, "PID: %d - Crear Segmento: %d - Base: %d - Tamanio: %d",pid,id_segmento,base_segmento,tamanio_segmento);
}
