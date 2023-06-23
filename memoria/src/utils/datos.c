#include "../../includes/datos.h"

void copiar_tabla_segmentos(void *stream, t_list *lista_tablas)
{
	int size_lista = list_size(lista_tablas);
	int offset = 0;

	for (int i = 0; i < size_lista; i++)
	{
		tabla_segmentos_t *tabla = list_get(lista_tablas, i);
		uint32_t size_tabla = list_size(tabla->segmentos);

		memcpy(stream + offset, &tabla->pid, sizeof(uint32_t));
		offset += sizeof(uint32_t);
		memcpy(stream + offset, &size_tabla, sizeof(uint32_t));
		offset += sizeof(uint32_t);

		for (int b = 0; b < size_tabla; b++)
		{
			segmento_t *segmento = list_get(tabla->segmentos, b);

			memcpy(stream + offset, &segmento->ids, sizeof(uint32_t));
			offset += sizeof(uint32_t);
			memcpy(stream + offset, &segmento->direccion_base, sizeof(uint32_t));
			offset += sizeof(uint32_t);
			memcpy(stream + offset, &segmento->tamanio, sizeof(uint32_t));
			offset += sizeof(uint32_t);
		}
	}
}

void copiar_instruccion_memoria(void *stream, t_instruc_mem *instruccion)
{
	int offset = 0;

	memcpy(stream + offset, &instruccion->estado, sizeof(contexto_estado_t));
	offset += sizeof(contexto_estado_t);

	memcpy(stream + offset, &instruccion->pid, sizeof(uint32_t));
	offset += sizeof(uint32_t);

	memcpy(stream + offset, &instruccion->param1_length, sizeof(uint32_t));
	offset += sizeof(uint32_t);
	memcpy(stream + offset, instruccion->param1, instruccion->param1_length);
	offset += instruccion->param1_length;

	memcpy(stream + offset, &instruccion->param2_length, sizeof(uint32_t));
	offset += sizeof(uint32_t);
	memcpy(stream + offset, instruccion->param2, instruccion->param2_length);
	offset += instruccion->param2_length;

	memcpy(stream + offset, &instruccion->param3_length, sizeof(uint32_t));
	offset += sizeof(uint32_t);
	memcpy(stream + offset, instruccion->param3, instruccion->param3_length);
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
