#include "../../../includes/fcb_list.h"

int obtener_cantidad_de_bloques(int id_fcb){
	int tamanio_fcb = valor_fcb(id_fcb, TAMANIO_ARCHIVO);
	int cant_bloques_fcb = ceil(tamanio_fcb / tamanio_de_bloque);

	return cant_bloques_fcb;
}
