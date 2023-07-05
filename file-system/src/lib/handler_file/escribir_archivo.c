#include "../../../includes/handler_file.h"

void escribir_dato(void* dato, int offset, int size){
	memcpy(memoria_file_system + offset,dato, size);

	sleep(retardo_acceso_bloque);
}

void escribir_int(uint32_t dato,int offset){
	memcpy(memoria_file_system + offset, &dato, 4);

	sleep(retardo_acceso_bloque);
}

//no usar no tiene retardo
void _escribir_int(uint32_t dato, int offset){
	memcpy(memoria_file_system + offset, &dato, 4);
}

void escribir_datos(void* datos, t_list* lista_offsets){
	int cant_bloques = list_size(lista_offsets);
	int offset = 0;

	for(int i = 0; i<cant_bloques; i++){
		offset_fcb_t* bloque = list_get(lista_offsets,i);

		escribir_dato(datos + offset, bloque->offset, bloque->tamanio);
		offset += bloque->tamanio;
	}
}
