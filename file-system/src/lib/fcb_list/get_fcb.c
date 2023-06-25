#include "../../../includes/fcb_list.h"

fcb_t* _get_fcb(char* nombre){
	return _get_fcb_id(buscar_fcb(nombre));
}

fcb_t* _get_fcb_id(int id){
	fcb_t* resultado = malloc(sizeof(fcb_t));

	int size = list_size(lista_global_fcb->lista_fcb);

	for(int i = 0; i<size; i++){
		fcb_t* fcb = list_get(lista_global_fcb->lista_fcb,i);

		if(fcb->id == id){
			resultado = fcb;
			break;
		}
	}

	return resultado;
}
