#include "../../includes/calculos.h"

int calcular_tam_total(t_list *lista_tablas)
{
	int size_total = 0;
	int size_lista = list_size(lista_tablas);

	for (int i = 0; i < size_lista; i++)
	{
		tabla_segmentos_t *tabla = list_get(lista_tablas, i);

		int size_tabla = list_size(tabla->segmentos);

		size_total += sizeof(uint32_t) * 3 * size_tabla;
		size_total += sizeof(uint32_t) * 2;
	}

	return size_total;
}

uint32_t calcular_tam_instruc_mem(t_instruc_mem *instruccion)
{
	uint32_t size = 0;

	size = sizeof(contexto_estado_t) + sizeof(uint32_t) + sizeof(uint32_t) + instruccion->param1_length + sizeof(uint32_t) + instruccion->param2_length + sizeof(uint32_t) + instruccion->param3_length;

	return size;
}

uint32_t calcular_tam_instruc_mov(t_instruc_mov *instruccion)
{
	uint32_t size = 0;

	size = sizeof(contexto_estado_t) + sizeof(uint32_t) + sizeof(uint32_t) + instruccion->param1_length + sizeof(uint32_t) + instruccion->param2_length + sizeof(uint32_t) + instruccion->param3_length;

	return size;
}
