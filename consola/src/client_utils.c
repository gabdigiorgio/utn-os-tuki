#include "../includes/client_utils.h"

void serializar_instrucciones(int socket, t_list* lista){
	t_buffer* buffer = malloc(sizeof(t_buffer));
	t_instruc* instrucciones = malloc(sizeof(t_instruc));

	int offset = 0;
	int lineas = list_size(lista);

	buffer->size = 0;

	for(int i = 0; i < lineas; i++){
		instrucciones = list_get(lista, i);

		buffer->size = buffer->size + sizeof(uint32_t)
				+ sizeof(uint32_t)
				+ instrucciones->instruct_length
				+ sizeof(uint32_t)
				+ instrucciones->param1_length
				+ sizeof(uint32_t)
				+ instrucciones->param2_length
				+ sizeof(uint32_t)
				+ instrucciones->param3_length;
	}

	void* stream = malloc(buffer->size);

	for(int i = 0; i < lineas; i++){
		instrucciones = list_get(lista, i);

		memcpy(stream + offset, &instrucciones->nro, sizeof(uint32_t));
		offset += sizeof(uint32_t);
		memcpy(stream + offset, &instrucciones->instruct_length, sizeof(uint32_t));
		offset += sizeof(uint32_t);
		memcpy(stream + offset, instrucciones->instruct, instrucciones->instruct_length);
		offset += instrucciones->instruct_length;
		memcpy(stream + offset, &instrucciones->param1_length, sizeof(uint32_t));
		offset += sizeof(uint32_t);
		memcpy(stream + offset, instrucciones->param1, instrucciones->param1_length);
		offset += instrucciones->param1_length;
		memcpy(stream + offset, &instrucciones->param2_length, sizeof(uint32_t));
		offset += sizeof(uint32_t);
		memcpy(stream + offset, instrucciones->param2, instrucciones->param2_length);
		offset += instrucciones->param2_length;
		memcpy(stream + offset, &instrucciones->param3_length, sizeof(uint32_t));
		offset += sizeof(uint32_t);
		memcpy(stream + offset, instrucciones->param3, instrucciones->param3_length);
		if(i != lineas) offset += instrucciones->param3_length;
	}

	buffer->stream = stream;

	free(instrucciones);

	t_paquete* paquete = malloc(sizeof(t_paquete));

	paquete->codigo_operacion = 1;
	paquete->buffer = buffer;
	paquete->lineas = lineas;

	void* a_enviar = malloc(buffer->size + sizeof(uint32_t) + sizeof(uint32_t) + sizeof(uint32_t));
	offset = 0;

	memcpy(a_enviar + offset, &(paquete->codigo_operacion), sizeof(uint32_t));
	offset += sizeof(uint32_t);
	memcpy(a_enviar + offset, &(paquete->lineas), sizeof(uint32_t));
	offset += sizeof(uint32_t);
	memcpy(a_enviar + offset, &(paquete->buffer->size), sizeof(uint32_t));
	offset += sizeof(uint32_t);
	memcpy(a_enviar + offset, paquete->buffer->stream, paquete->buffer->size);

	send(socket, a_enviar, buffer->size + sizeof(uint32_t) + sizeof(uint32_t) + sizeof(uint32_t), 0);

	free(a_enviar);
	free(paquete->buffer->stream);
	free(paquete->buffer);
	free(paquete);
}

int crear_conexion(char *ip, char* puerto)
{
	struct addrinfo hints;
	struct addrinfo *server_info;

	memset(&hints, 0, sizeof(hints));
	hints.ai_family = AF_UNSPEC;
	hints.ai_socktype = SOCK_STREAM;
	hints.ai_flags = AI_PASSIVE;

	getaddrinfo(ip, puerto, &hints, &server_info);

	// Ahora creo el socket.
	int socket_cliente = socket(server_info->ai_family,server_info->ai_socktype,server_info->ai_protocol);

	// Ahora que tenemos el socket, vamos a conectarlo
    if(connect(socket_cliente,server_info->ai_addr,server_info->ai_addrlen) != 0){
    	puts("Error al conectar el socket del cliente");
    	exit(1);
    };

	freeaddrinfo(server_info);
	return socket_cliente;
}

void liberar_conexion(int socket_cliente)
{
	close(socket_cliente);
}
