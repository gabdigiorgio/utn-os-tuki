#include "../../includes/calculos.h"

uint32_t calcular_tam_instrucciones(t_list* lista){
	uint32_t size = 0;
	int lineas = list_size(lista);

	for(int i = 0; i < lineas; i++){
			t_instruc* instrucciones = list_get(lista, i);

			size = size + sizeof(uint32_t)
					+ sizeof(uint32_t)
					+ instrucciones->instruct_length
					+ sizeof(uint32_t)
					+ instrucciones->param1_length
					+ sizeof(uint32_t)
					+ instrucciones->param2_length
					+ sizeof(uint32_t)
					+ instrucciones->param3_length;
		}


	return size;
}

uint32_t calcular_tam_registros(t_registros* registros){
	uint32_t size = 0;

	size = sizeof(uint16_t) + sizeof(char) * 20 + sizeof(char) * 36 + sizeof(char) * 68;

	return size;
}

uint32_t calcular_tam_contexto(t_contexto* contexto){
	uint32_t size = 0;

	size = sizeof(uint32_t) + sizeof(contexto_estado_t) + sizeof(uint32_t) + contexto->param1_length + sizeof(uint32_t) + contexto->param2_length + sizeof(uint32_t) + contexto->param3_length;

	return size;
}

uint32_t calcular_tam_tabla_segmentos(tabla_segmentos_t * tabla_segmento)
{
	uint32_t size = sizeof(uint32_t) + sizeof(uint32_t); //size del PID + size de la lista

	for (int i = 0; i < list_size(tabla_segmento->segmentos); i++) //size de los elementos de la lista
	{
		size += sizeof(uint32_t) * 3;
	}

	return size;
}

uint32_t calcular_tam_instruc_mem(t_instruc_mem* instruccion){
	uint32_t size = 0;

	size = sizeof(contexto_estado_t) +
			sizeof(uint32_t) +
			sizeof(uint32_t) + instruccion->param1_length +
			sizeof(uint32_t) + instruccion->param2_length +
			sizeof(uint32_t) + instruccion->param3_length;

	return size;
}
