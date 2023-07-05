#include "../../../includes/handler_file.h"

void* leer_dato(int offset, int size){
	void* dato = malloc(size);

	memcpy(dato,memoria_file_system + offset, size);

	sleep(retardo_acceso_bloque);

	return dato;
}

uint32_t leer_int(int offset, int size){
	uint32_t dato = 0;

	memcpy(&dato,memoria_file_system + offset, size);

	sleep(retardo_acceso_bloque);

	return dato;
}
//no usar no tiene retardo
uint32_t _leer_int(int offset, int size){
	uint32_t dato = 0;

	memcpy(&dato,memoria_file_system + offset, size);

	return dato;
}

void leer_bloques_indirectos(int id_fcb,t_list* lista_de_bloques, int offset_inicial, int offset_final){
	int offset_fcb = offset_inicial;
	uint32_t bloque_indirecto = valor_fcb(id_fcb, PUNTERO_INDIRECTO);
	int offset = floor(((double)offset_fcb / tamanio_de_bloque) - 1) * sizeof(uint32_t);

	while(offset_fcb < offset_final){
		offset_fcb_t *bloque = malloc(sizeof(offset_fcb_t));
		uint32_t dato = _leer_int((bloque_indirecto * tamanio_de_bloque) + offset, sizeof(uint32_t));
		memcpy(&bloque->id_bloque, &dato , sizeof(uint32_t));
		bloque->offset = bloque->id_bloque * tamanio_de_bloque;
		list_add(lista_de_bloques, bloque);
		offset += sizeof(uint32_t);
		offset_fcb += tamanio_de_bloque;
	}

	sleep(retardo_acceso_bloque);
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
