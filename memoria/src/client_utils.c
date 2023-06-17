#include "../includes/client_utils.h"

void crear_header(void *a_enviar, t_buffer *buffer, uint32_t lineas)
{
	//Reservo el stream para el header del paquete
	int offset = 0;
	uint32_t i = 1;

	//Añado los datos del header al stream
	memcpy(a_enviar + offset, &(i), sizeof(uint32_t));
	offset += sizeof(uint32_t);
	memcpy(a_enviar + offset, &(lineas), sizeof(uint32_t));
	offset += sizeof(uint32_t);
	memcpy(a_enviar + offset, &(buffer->size), sizeof(uint32_t));
	offset += sizeof(uint32_t);
	memcpy(a_enviar + offset, buffer->stream, buffer->size);
}

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

void serializar_tabla_segmentos(int socket_cliente, t_list *lista_tablas)
{
	t_buffer *buffer = malloc(sizeof(t_buffer));

	buffer->size = calcular_tam_total(lista_tablas);

	void *stream = malloc(buffer->size);

	copiar_tabla_segmentos(stream, lista_tablas);

	buffer->stream = stream;

	void *a_enviar = malloc(buffer->size + sizeof(uint32_t) * 3);

	crear_header(a_enviar, buffer, list_size(lista_tablas));

	send(socket_cliente, a_enviar, buffer->size + sizeof(uint32_t) + sizeof(uint32_t) + sizeof(uint32_t), 0);

	free(buffer->stream);
	free(buffer);
	free(a_enviar);
}
void serializar_respuesta_memoria_kernel(int socket_cliente, t_resp_mem respuesta)
{
	t_buffer *buffer = malloc(sizeof(t_buffer));

	buffer->size = sizeof(t_resp_mem);

	void *stream = malloc(buffer->size);

	memcpy(stream, &respuesta, buffer->size);

	buffer->stream = stream;

	void *a_enviar = malloc(buffer->size + sizeof(uint32_t) * 3);

	crear_header(a_enviar, buffer, 0);

	send(socket_cliente, a_enviar, buffer->size + sizeof(uint32_t) + sizeof(uint32_t) + sizeof(uint32_t), 0);

	// Liberar memoria utilizada
	free(buffer->stream);
	free(buffer);
	free(a_enviar);

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

uint32_t calcular_tam_instruc_mem(t_instruc_mem *instruccion)
{
	uint32_t size = 0;

	size = sizeof(contexto_estado_t) + sizeof(uint32_t) + sizeof(uint32_t) + instruccion->param1_length + sizeof(uint32_t) + instruccion->param2_length + sizeof(uint32_t) + instruccion->param3_length;

	return size;
}

void serializar_instruccion_memoria(int socket, t_instruc_mem *instruccion)
{
	//Creo el buffer a utilizar para las instrucciones
	t_buffer *buffer = malloc(sizeof(t_buffer));

	buffer->size = calcular_tam_instruc_mem(instruccion);

	//Asigno memoria para el stream del tamaño de mi lista
	void *stream = malloc(buffer->size);

	//Leo toda la lista para copiar los valores en memoria
	copiar_instruccion_memoria(stream, instruccion);

	//Añado el stream a mi buffers
	buffer->stream = stream;

	void *a_enviar = malloc(buffer->size + sizeof(uint32_t) * 3);

	crear_header(a_enviar, buffer, 0);

	//Envio todo el stream al servidor
	send(socket, a_enviar, buffer->size + sizeof(uint32_t) + sizeof(uint32_t) + sizeof(uint32_t), 0);

	//Libero memoria que ya no voy a utilizar
	free(buffer->stream);
	free(buffer);
	free(a_enviar);
}
