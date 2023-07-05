#include "../../../includes/handler_file.h"

void* leer_dato(int offset, int size){
	void* dato = malloc(size);

	memcpy(dato,memoria_file_system + offset, size);

	usleep(retardo_acceso_bloque);

	return dato;
}

uint32_t leer_int(int offset, int size){
	uint32_t dato = 0;

	memcpy(&dato,memoria_file_system + offset, size);

	usleep(retardo_acceso_bloque);

	return dato;
}

void* leer_datos(t_list* lista_offsets){
	int cant_bloques = list_size(lista_offsets);
	int offset = 0;
	offset_fcb_t* bloque_inicial = list_get(lista_offsets,0);

	void* dato = leer_dato(bloque_inicial->offset,bloque_inicial->tamanio);

	void* datos = malloc(bloque_inicial->tamanio);

	memcpy(datos,dato,bloque_inicial->tamanio);
	offset += bloque_inicial->tamanio;

	for(int i = 1; i<cant_bloques; i++){
		offset_fcb_t* bloque = list_get(lista_offsets,i);

		void* dato = leer_dato(bloque->offset,bloque->tamanio);

		datos = realloc(datos,offset + bloque->tamanio);
		memcpy(datos+offset,dato,bloque->tamanio);
		offset += bloque->tamanio;
	}

	return datos;
}
