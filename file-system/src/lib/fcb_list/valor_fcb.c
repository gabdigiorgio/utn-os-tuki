#include "../../../includes/fcb_list.h"

uint32_t valor_fcb(int id,fcb_prop_t llave){
	fcb_t* fcb = _get_fcb_id(id);

	uint32_t valor = _valor_fcb(fcb,llave);

	return valor;
}

uint32_t _valor_fcb(fcb_t* fcb, fcb_prop_t llave){
	uint32_t valor = 0;

	switch(llave){
		case TAMANIO_ARCHIVO:
			valor = fcb->tamanio_archivo;
			break;
		case PUNTERO_DIRECTO:
			valor = fcb->puntero_directo;
			break;
		case PUNTERO_INDIRECTO:
			valor = fcb->puntero_indirecto;
			break;
		case PUNTERO_ARCHIVO:
			valor = fcb->puntero_archivo;
			break;
		default:
			break;
	}

	return valor;
}
