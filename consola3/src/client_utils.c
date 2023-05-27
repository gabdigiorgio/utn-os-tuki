#include "../includes/client_utils.h"

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

void copiar_instrucciones(void* stream, t_list* lista){
	int lineas = list_size(lista);
	int offset = 0;

	for(int i = 0; i < lineas; i++){
			t_instruc* instrucciones = list_get(lista, i);

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
}

void crear_header(void* a_enviar, t_buffer* buffer, int lineas){
	uint32_t codigo_operacion = 1;

	//Reservo el stream para el header del paquete
	int offset = 0;

	//Añado los datos del header al stream
	memcpy(a_enviar + offset, &(codigo_operacion), sizeof(uint32_t));
	offset += sizeof(uint32_t);
	memcpy(a_enviar + offset, &(lineas), sizeof(uint32_t));
	offset += sizeof(uint32_t);
	memcpy(a_enviar + offset, &(buffer->size), sizeof(uint32_t));
	offset += sizeof(uint32_t);
	memcpy(a_enviar + offset, buffer->stream, buffer->size);

}

void serializar_instrucciones(int socket, t_list* lista){
	//Creo el buffer a utilizar para las instrucciones
	t_buffer* buffer = malloc(sizeof(*buffer));
	buffer->size = 0;
	//t_instruc* instrucciones = malloc(sizeof(t_instruc));

	//Leo la lista de instrucciones para sumar el tamaño de toda la lista
	buffer->size = calcular_tam_instrucciones(lista);

	//Asigno memoria para el stream del tamaño de mi lista
	void* stream = malloc(buffer->size);

	//Leo toda la lista para copiar los valores en memoria
	copiar_instrucciones(stream,lista);

	//Añado el stream a mi buffer
	buffer->stream = stream;

	void* a_enviar = malloc(buffer->size + sizeof(uint32_t) * 3);

	crear_header(a_enviar,buffer,list_size(lista));
	//Creo un paquete y le asigno los valores iniciales

	//Envio todo el stream al servidor
	send(socket, a_enviar, buffer->size + sizeof(uint32_t) + sizeof(uint32_t) + sizeof(uint32_t), 0);

	//Libero memoria que ya no voy a utilizar
	free(buffer->stream);
	free(buffer);
	free(a_enviar);
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

uint8_t recibir_respuesta(int socket_cliente){
	uint8_t result = 0;

	recv(socket_cliente, &result, sizeof(uint8_t), MSG_WAITALL);

	return result;
}

void liberar_conexion(int socket_cliente)
{
	close(socket_cliente);
}
