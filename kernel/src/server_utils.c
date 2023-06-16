#include "../includes/server_utils.h"

int iniciar_servidor(char* puerto)
{

	int socket_servidor;
	struct addrinfo hints, *servinfo, *p;

	memset(&hints, 0, sizeof(hints));
	hints.ai_family = AF_UNSPEC;
	hints.ai_socktype = SOCK_STREAM;
	hints.ai_flags = AI_PASSIVE;

	getaddrinfo(NULL, puerto, &hints, &servinfo);

	// Creamos el socket de escucha del servidor
	socket_servidor = socket(servinfo->ai_family,servinfo->ai_socktype,servinfo->ai_protocol);

	// Asociamos el socket a un puerto
	bind(socket_servidor, servinfo->ai_addr,servinfo->ai_addrlen);

	// Escuchamos las conexiones entrantes
	listen(socket_servidor, SOMAXCONN);


	freeaddrinfo(servinfo);
	log_trace(logger, "Listo para escuchar a mi cliente");

	return socket_servidor;
}

int esperar_cliente(int socket_servidor)
{
	// Aceptamos un nuevo cliente
	int socket_cliente = accept(socket_servidor, NULL, NULL);
	log_info(logger, "Se conecto un cliente!");

	return socket_cliente;
}

void deserializar_header(t_paquete* paquete, int socket_cliente){
	recv(socket_cliente, &(paquete->codigo_operacion), sizeof(uint32_t), MSG_WAITALL);
	recv(socket_cliente, &(paquete->lineas), sizeof(uint32_t), MSG_WAITALL);
	recv(socket_cliente, &(paquete->buffer->size), sizeof(uint32_t), MSG_WAITALL);
	paquete->buffer->stream = malloc(paquete->buffer->size);
	recv(socket_cliente, paquete->buffer->stream, paquete->buffer->size, MSG_WAITALL);
}

void deserializar_instrucciones(t_list* lista, t_buffer* buffer, int lineas){
	void* stream = buffer->stream;

	for(int i=0; i<lineas; i++){
		t_instruc* instrucciones = malloc(sizeof *instrucciones);

		memcpy(&(instrucciones->nro), stream, sizeof(uint32_t));
		stream += sizeof(uint32_t);

		memcpy(&(instrucciones->instruct_length), stream, sizeof(uint32_t));
		stream += sizeof(uint32_t);

		instrucciones->instruct = malloc(instrucciones->instruct_length);
		memcpy(instrucciones->instruct, stream, instrucciones->instruct_length);
		stream += instrucciones->instruct_length;

		memcpy(&(instrucciones->param1_length), stream, sizeof(uint32_t));
		stream += sizeof(uint32_t);

		instrucciones->param1 = malloc(instrucciones->param1_length);
		memcpy(instrucciones->param1, stream, instrucciones->param1_length);
		stream += instrucciones->param1_length;

		memcpy(&(instrucciones->param2_length), stream, sizeof(uint32_t));
		stream += sizeof(uint32_t);

		instrucciones->param2 = malloc(instrucciones->param2_length);
		memcpy(instrucciones->param2, stream, instrucciones->param2_length);
		stream += instrucciones->param2_length;

		memcpy(&(instrucciones->param3_length), stream, sizeof(uint32_t));
		stream += sizeof(uint32_t);

		instrucciones->param3 = malloc(instrucciones->param3_length);
		memcpy(instrucciones->param3, stream, instrucciones->param3_length);
		stream += instrucciones->param3_length;

		list_add(lista, instrucciones);
	}
}

void deserializar_contexto(t_contexto* contexto, t_buffer* buffer, int lineas){
	void* stream = buffer->stream;

	for(int i=0; i<lineas; i++){
			t_instruc* instrucciones = malloc(sizeof *instrucciones);

			memcpy(&(instrucciones->nro), stream, sizeof(uint32_t));
			stream += sizeof(uint32_t);

			memcpy(&(instrucciones->instruct_length), stream, sizeof(uint32_t));
			stream += sizeof(uint32_t);

			instrucciones->instruct = malloc(instrucciones->instruct_length);
			memcpy(instrucciones->instruct, stream, instrucciones->instruct_length);
			stream += instrucciones->instruct_length;

			memcpy(&(instrucciones->param1_length), stream, sizeof(uint32_t));
			stream += sizeof(uint32_t);

			instrucciones->param1 = malloc(instrucciones->param1_length);
			memcpy(instrucciones->param1, stream, instrucciones->param1_length);
			stream += instrucciones->param1_length;

			memcpy(&(instrucciones->param2_length), stream, sizeof(uint32_t));
			stream += sizeof(uint32_t);

			instrucciones->param2 = malloc(instrucciones->param2_length);
			memcpy(instrucciones->param2, stream, instrucciones->param2_length);
			stream += instrucciones->param2_length;

			memcpy(&(instrucciones->param3_length), stream, sizeof(uint32_t));
			stream += sizeof(uint32_t);

			instrucciones->param3 = malloc(instrucciones->param3_length);
			memcpy(instrucciones->param3, stream, instrucciones->param3_length);
			stream += instrucciones->param3_length;

			list_add(contexto->instrucciones, instrucciones);
		}

	memcpy(&(contexto->registros->ip), stream, sizeof(uint16_t));
	stream += sizeof(uint16_t);
	memcpy(&(contexto->registros->ax), stream, sizeof(char) * 5);
	stream += sizeof(char) * 5;
	memcpy(&(contexto->registros->bx), stream, sizeof(char) * 5);
	stream += sizeof(char) * 5;
	memcpy(&(contexto->registros->cx), stream, sizeof(char) * 5);
	stream += sizeof(char) * 5;
	memcpy(&(contexto->registros->dx), stream, sizeof(char) * 5);
	stream += sizeof(char) * 5;
	memcpy(&(contexto->registros->eax), stream, sizeof(char) * 9);
	stream += sizeof(char) * 9;
	memcpy(&(contexto->registros->ebx), stream, sizeof(char) * 9);
	stream += sizeof(char) * 9;
	memcpy(&(contexto->registros->ecx), stream, sizeof(char) * 9);
	stream += sizeof(char) * 9;
	memcpy(&(contexto->registros->edx), stream, sizeof(char) * 9);
	stream += sizeof(char) * 9;
	memcpy(&(contexto->registros->rax), stream, sizeof(char) * 17);
	stream += sizeof(char) * 17;
	memcpy(&(contexto->registros->rbx), stream, sizeof(char) * 17);
	stream += sizeof(char) * 17;
	memcpy(&(contexto->registros->rcx), stream, sizeof(char) * 17);
	stream += sizeof(char) * 17;
	memcpy(&(contexto->registros->rdx), stream, sizeof(char) * 17);
	stream += sizeof(char) * 17;
	memcpy(&(contexto->pid), stream, sizeof(uint32_t));
	stream += sizeof(uint32_t);
	memcpy(&(contexto->estado), stream, sizeof(contexto_estado_t));
	stream += sizeof(contexto_estado_t);

	memcpy(&(contexto->param1_length), stream, sizeof(uint32_t));
	stream += sizeof(uint32_t);
	contexto->param1 = realloc(contexto->param1,contexto->param1_length);
	memcpy(contexto->param1, stream, contexto->param1_length);
	stream += contexto->param1_length;
	memcpy(&(contexto->param2_length), stream, sizeof(uint32_t));
	stream += sizeof(uint32_t);
	contexto->param2 = realloc(contexto->param2,contexto->param2_length);
	memcpy(contexto->param2, stream, contexto->param2_length);
	stream += contexto->param2_length;

	memcpy(&(contexto->param3_length), stream, sizeof(uint32_t));
	stream += sizeof(uint32_t);
	contexto->param3 = realloc(contexto->param3,contexto->param3_length);
	memcpy(contexto->param3, stream, contexto->param3_length);
}

void deserializar_tabla_segmentos(t_lista_mutex* lista_tablas, t_buffer* buffer, int lineas){
	void* stream = buffer->stream;
	int size_actual = list_size(lista_tablas->lista);

	for(int i=0 ; i<lineas ; i++){
		tabla_segmentos_t* tabla_segmentos = malloc(sizeof(tabla_segmentos_t));
		tabla_segmentos->segmentos = list_create();
		uint32_t size_tabla = 0;

		memcpy(&tabla_segmentos->pid, stream, sizeof(uint32_t));
		stream += sizeof(uint32_t);
		memcpy(&size_tabla, stream, sizeof(uint32_t));
		stream += sizeof(uint32_t);

		for(uint32_t b = 0 ; b<size_tabla ; b++){
			segmento_t* segmento = malloc(sizeof(segmento_t));

			memcpy(&segmento->ids, stream, sizeof(uint32_t));
			stream += sizeof(uint32_t);
			memcpy(&segmento->direccion_base, stream, sizeof(uint32_t));
			stream += sizeof(uint32_t);
			memcpy(&segmento->tamanio, stream, sizeof(uint32_t));
			stream += sizeof(uint32_t);

			list_add(tabla_segmentos->segmentos,segmento);
		}

		if(existe_tabla_segmentos(lista_tablas->lista,tabla_segmentos->pid)){
			tabla_segmentos_t* tabla_existente = buscar_tabla_segmentos(lista_tablas->lista,tabla_segmentos->pid);
			t_list* segmentos_a_borrar = list_create();
			segmentos_a_borrar = tabla_existente->segmentos;
			tabla_existente->segmentos = tabla_segmentos->segmentos;

			list_destroy_and_destroy_elements(segmentos_a_borrar,free);
		} else {
			list_push(lista_tablas,tabla_segmentos);
		}
	}
}
/*void deserializar_respuesta_memoria(t_resp_mem respuesta, t_buffer* buffer, int lineas){
	void* stream = buffer->stream;

	memcpy(&respuesta, stream, sizeof(t_resp_mem));
	stream += sizeof(t_resp_mem);


}
*/

t_resp_mem deserializar_respuesta_memoria(t_buffer* buffer) {
    void* stream = buffer->stream;
    t_resp_mem respuesta = SUCCESS_CREATE_SEGMENT;

    memcpy(&respuesta, stream, sizeof(t_resp_mem));

   return respuesta;
}
