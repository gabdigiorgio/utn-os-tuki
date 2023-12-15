#include "../../../includes/fcb_list.h"

int nombre_fcb(int id,char* nombre){
	int resultado = buscar_fcb(id);

	if(resultado != -1){
		fcb_t* fcb = _get_fcb_id(id);
		int size_nombre = strlen(fcb->nombre_archivo);

		nombre = realloc(nombre,size_nombre);
		memcpy(nombre,fcb->nombre_archivo,size_nombre + 1);
	}

	return resultado;
}
