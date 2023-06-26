#include "../../../includes/fcb_list.h"

int borrar_fcb(int id){
	int resultado = -1;

	if(buscar_fcb_id(id) == -1){
		log_error(logger,"No existe un FCB con ese nombre");
		return resultado;
	}

	fcb_t* fcb = _get_fcb_id(id);

	list_remove_element(lista_global_fcb->lista_fcb,fcb);
	remove(fcb->ruta_archivo);

	free(fcb->nombre_archivo);
	free(fcb->ruta_archivo);
	free(fcb);

	return resultado;
}
