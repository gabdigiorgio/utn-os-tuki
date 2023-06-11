#include "../includes/utils.h"

t_config* iniciar_config(char * path_config)
{
	t_config* nuevo_config;
	if((nuevo_config = config_create(path_config))==NULL){
		printf("No pude leer la config \n");
		exit(2);
	}
	return nuevo_config;
}

t_log* iniciar_logger(void)
{
	t_log* nuevo_logger = log_create("memoria.log", "MEMORIA", 1, LOG_LEVEL_DEBUG);

	return nuevo_logger;
}

t_instruc_mem* inicializar_instruc_mem()
{
	t_instruc_mem* contexto = malloc(sizeof(t_instruc_mem));
	contexto->pid=0;
	contexto->param1 = malloc(sizeof(char) * 2);
	memcpy(contexto->param1, "0", (sizeof(char) * 2));
	contexto->param1_length = sizeof(char) * 2;
	contexto->param2 = malloc(sizeof(char) * 2);
	memcpy(contexto->param2, "0", (sizeof(char) * 2));
	contexto->param2_length = sizeof(char) * 2;
	contexto->param3 = malloc(sizeof(char) * 2);
	memcpy(contexto->param3, "0", (sizeof(char) * 2));
	contexto->param3_length = sizeof(char) * 2;
	contexto->estado = CREATE_SEGMENT;

	return contexto;
}

void copiar_instruccion_mem(t_instruc_mem* instruccion, t_contexto* contexto){
	instruccion->param1_length = contexto->param1_length;
	instruccion->param2_length = contexto->param2_length;
	instruccion->param3_length = contexto->param3_length;

	instruccion->param1 = realloc(instruccion->param1,instruccion->param1_length);
	instruccion->param2 = realloc(instruccion->param2,instruccion->param2_length);
	instruccion->param3 = realloc(instruccion->param3,instruccion->param3_length);

	memcpy(instruccion->param1,contexto->param1,instruccion->param1_length);
	memcpy(instruccion->param2,contexto->param2,instruccion->param2_length);
	memcpy(instruccion->param3,contexto->param3,instruccion->param3_length);
	memcpy(&(instruccion->pid), &(contexto->pid), sizeof(uint32_t));
}

void reducir_huecos(t_list* lista_huecos){
	int pos = 0;

	while(list_size(lista_huecos) > 1 && pos < (list_size(lista_huecos) - 1)){
		hueco_libre_t* hueco1 = list_remove(lista_huecos,pos);
		hueco_libre_t* hueco2 = list_remove(lista_huecos,pos);
		if((hueco1->direccion_base + hueco1->tamanio) == hueco2->direccion_base){
			hueco_libre_t* nuevo_hueco = malloc(sizeof(hueco_libre_t));

			nuevo_hueco->direccion_base = hueco1->direccion_base;
			nuevo_hueco->tamanio = hueco1->tamanio + hueco2->tamanio;

			list_add_in_index(lista_huecos,pos,nuevo_hueco);
			free(hueco1);
			free(hueco2);
		} else {
			list_add_in_index(lista_huecos,pos,hueco2);
			list_add_in_index(lista_huecos,pos,hueco1);
			pos++;
		}
	}
}

//no usar malloc
void eliminar_segmento(t_list* lista_segmentos, t_list* lista_huecos, uint32_t id_segmento){

	bool comparar_segmento(segmento_t* seg){
		return seg->ids == id_segmento;
	}

	segmento_t* segmento = list_find(lista_segmentos, comparar_segmento);

	list_remove_element(lista_segmentos, segmento);

	hueco_libre_t* nuevo_hueco = malloc(sizeof(hueco_libre_t));
	nuevo_hueco->direccion_base = segmento->direccion_base;
	nuevo_hueco->tamanio = segmento->tamanio;

	list_add(lista_huecos,nuevo_hueco);

	tam_memoria_restante += segmento->tamanio;

	free(segmento);

	list_sort(lista_huecos,(void*) ordenar_lista_huecos);

	reducir_huecos(lista_huecos);

}

//no usar malloc
void compactar_memoria(t_list* lista_segmentos, t_list* lista_huecos){
	int max_size = 0;
	int size_segmentos = list_size(lista_segmentos);
	int size_huecos = list_size(lista_huecos);

	segmento_t* segmento_max = list_get(lista_segmentos, size_segmentos - 1 );
	hueco_libre_t* hueco_max = list_get(lista_huecos, size_huecos - 1);

	max_size = segmento_max->direccion_base > hueco_max->direccion_base
			? (segmento_max->direccion_base + segmento_max->tamanio)
			: (hueco_max->direccion_base + hueco_max->tamanio);

	int last_tamanio = 0;

	for(int i=0;i<size_segmentos;i++){
		segmento_t* segmento = list_get(lista_segmentos,i);

		memcpy(memoria + last_tamanio,memoria + segmento->direccion_base, segmento->tamanio);
		//memcpy(memoria + base, asdasd, offset);

		//5646578676 base + offset, 54 5678 segmento + limite

		segmento->direccion_base = last_tamanio;
		last_tamanio = segmento->tamanio + last_tamanio;
	}

	segmento_t* nuevo_segmento_max = list_get(lista_segmentos, size_segmentos - 1);

	hueco_libre_t* nuevo_hueco = malloc(sizeof(hueco_libre_t));
	nuevo_hueco->direccion_base = nuevo_segmento_max->direccion_base + nuevo_segmento_max ->tamanio ;
	nuevo_hueco->tamanio = max_size - (nuevo_segmento_max->direccion_base + nuevo_segmento_max ->tamanio);

	while(size_huecos > 0){
		list_remove_and_destroy_element(lista_huecos,size_huecos - 1,free);
		size_huecos = list_size(lista_huecos);
	}
	list_add(lista_huecos,nuevo_hueco);
}

bool ordenar_lista_huecos(hueco_libre_t* hueco1, hueco_libre_t* hueco2){
	return hueco1->direccion_base < hueco2->direccion_base;
}

bool ordenar_lista_segmentos(segmento_t* segmento1, segmento_t* segmento2){
	return segmento1->direccion_base < segmento2->direccion_base;
}

void imprimir_valores_huecos(hueco_libre_t* hueco){
	log_info(logger,"Base hueco: %d, Tamanio hueco: %d",hueco->direccion_base,hueco->tamanio);
}

void imprimir_valores_segmentos(segmento_t* segmento){
	log_info(logger,"Id segmento: %d, Base segmento: %d, Tamanio segmento: %d",segmento->ids,segmento->direccion_base,segmento->tamanio);
}


