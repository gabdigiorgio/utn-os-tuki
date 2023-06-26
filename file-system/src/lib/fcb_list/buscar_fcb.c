#include "../../../includes/fcb_list.h"

int buscar_fcb(char* nombre_fcb){
	int resultado = -1;
	int size = list_size(lista_global_fcb->lista_fcb);

	for(int i = 0; i<size; i++){
		fcb_t* fcb = list_get(lista_global_fcb->lista_fcb,i);

		if(strcmp(fcb->nombre_archivo,nombre_fcb) == 0){
			resultado = fcb->id;
			break;
		}
	}

	return resultado;
}

int buscar_fcb_id(int id){
	int resultado = -1;
	int size = list_size(lista_global_fcb->lista_fcb);

	for(int i = 0; i<size; i++){
		fcb_t* fcb = list_get(lista_global_fcb->lista_fcb,i);

		if(fcb->id == id){
			resultado = fcb->id;
			break;
		}
	}

	return resultado;
}
