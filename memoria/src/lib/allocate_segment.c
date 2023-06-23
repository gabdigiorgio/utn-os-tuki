#include "../../includes/mem_instruct.h"

void allocate_segmento_0(int pid){

	tabla_segmentos_t* tabla_0 = list_get(lista_de_tablas,0);
	tabla_segmentos_t* tabla = malloc(sizeof(tabla_segmentos_t));

	tabla->pid = pid;
	tabla->segmentos = list_create();

	segmento_t* segmento_0 = list_get(tabla_0->segmentos,0);

	list_add(tabla->segmentos,segmento_0);
	list_add(lista_de_tablas,tabla);
}
