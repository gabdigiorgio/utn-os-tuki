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
	// Quitar esta línea cuando hayamos terminado de implementar la funcion
	//assert(!"no implementado!");

	// Aceptamos un nuevo cliente
	int socket_cliente = accept(socket_servidor, NULL, NULL);
	log_info(logger, "Se conecto un cliente!");

	return socket_cliente;
}

void deserializar_header(t_paquete* paquete, int socket){
	recv(socket, &(paquete->codigo_operacion), sizeof(uint32_t), 0);
	recv(socket, &(paquete->lineas), sizeof(uint32_t), 0);
	recv(socket, &(paquete->buffer->size), sizeof(uint32_t), 0);
	paquete->buffer->stream = malloc(paquete->buffer->size);
	recv(socket, paquete->buffer->stream, paquete->buffer->size, 0);
}

t_list* deserializar_instrucciones(t_buffer* buffer, int lineas){
	t_list* lista = malloc(sizeof(t_list));

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

	return lista;

}

t_contexto* deserializar_contexto(t_buffer* buffer, int lineas){
	t_list* lista = list_create();
	t_contexto* contexto = malloc(sizeof(t_contexto));

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

	t_registros* registros = malloc(sizeof *registros);

	memcpy(&(registros->ip), stream, sizeof(uint16_t));
	stream += sizeof(uint16_t);
	memcpy(&(registros->ax), stream, sizeof(char) * 5);
	stream += sizeof(char) * 5;
	memcpy(&(registros->bx), stream, sizeof(char) * 5);
	stream += sizeof(char) * 5;
	memcpy(&(registros->cx), stream, sizeof(char) * 5);
	stream += sizeof(char) * 5;
	memcpy(&(registros->dx), stream, sizeof(char) * 5);
	stream += sizeof(char) * 5;
	memcpy(&(registros->eax), stream, sizeof(char) * 9);
	stream += sizeof(char) * 9;
	memcpy(&(registros->ebx), stream, sizeof(char) * 9);
	stream += sizeof(char) * 9;
	memcpy(&(registros->ecx), stream, sizeof(char) * 9);
	stream += sizeof(char) * 9;
	memcpy(&(registros->edx), stream, sizeof(char) * 9);
	stream += sizeof(char) * 9;
	memcpy(&(registros->rax), stream, sizeof(char) * 17);
	stream += sizeof(char) * 17;
	memcpy(&(registros->rbx), stream, sizeof(char) * 17);
	stream += sizeof(char) * 17;
	memcpy(&(registros->rcx), stream, sizeof(char) * 17);
	stream += sizeof(char) * 17;
	memcpy(&(registros->rdx), stream, sizeof(char) * 17);
	stream += sizeof(char) * 17;

	contexto->instrucciones = lista;
	contexto->registros = registros;

	return contexto;
}



