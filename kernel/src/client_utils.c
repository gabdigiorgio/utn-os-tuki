#include "../includes/client_utils.h"

int crear_conexion(char *ip, char* puerto)
{
	struct addrinfo hints;
	struct addrinfo *server_info;

	memset(&hints, 0, sizeof(hints));
	hints.ai_family = AF_UNSPEC;
	hints.ai_socktype = SOCK_STREAM;
	hints.ai_flags = AI_PASSIVE;

	getaddrinfo(ip, puerto, &hints, &server_info);

	// Ahora vamos a crear el socket.
	int socket_cliente = socket(server_info->ai_family,server_info->ai_socktype,server_info->ai_protocol);

	// Ahora que tenemos el socket, vamos a conectarlo
	connect(socket_cliente, server_info->ai_addr, server_info->ai_addrlen);

	freeaddrinfo(server_info);

	return socket_cliente;
}

int handshake(int socket_cliente, uint8_t tipo_cliente, uint8_t tipo_servidor){ //char* handshake (int socket_cliente, uint8_t tipo_cliente)

	char* message = "";
	uint8_t handshake = tipo_cliente;
	uint8_t result;

	switch(tipo_servidor){
				case 1:
					message = "Cpu";
					break;
				case 2:
					message = "Kernel";
					break;
				case 3:
					message = "File system";
					break;
				case 4:
					message = "Memoria";
					break;
				default:
					break;
			}

	send(socket_cliente, &handshake, sizeof(uint8_t), NULL);
	log_info(logger, message);
	recv(socket_cliente, &result, sizeof(uint8_t), MSG_WAITALL); //consultar por un timeout

	if(result == 1){
		log_info(logger, "Se establecio correctamente la conexion");
	} else {
		log_info(logger, "Fallo al realizar el handshake, cerrando conexion");
		result = -1;
	}

	return result;
}

uint32_t calcular_tam_instrucciones(t_list* lista){
	t_instruc* instrucciones = malloc(sizeof(t_instruc));
	uint32_t size = 0;
	int lineas = list_size(lista);

	for(int i = 0; i < lineas; i++){
			instrucciones = list_get(lista, i);

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

void copiar_contexto(void* stream, t_list* lista, t_registros* registros){
	t_instruc* instrucciones = malloc(sizeof(t_instruc));
	int lineas = list_size(lista);
	int offset = 0;

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
			offset += instrucciones->param3_length;
		}

	memcpy(stream + offset, &registros->ip, sizeof(uint16_t));
	offset += sizeof(uint16_t);
	memcpy(stream + offset, registros->ax, sizeof(char) * 5);
	offset += sizeof(char) * 5;
	memcpy(stream + offset, registros->bx, sizeof(char) * 5);
	offset += sizeof(char) * 5;
	memcpy(stream + offset, registros->cx, sizeof(char) * 5);
	offset += sizeof(char) * 5;
	memcpy(stream + offset, registros->dx, sizeof(char) * 5);
	offset += sizeof(char) * 5;
	memcpy(stream + offset, registros->eax, sizeof(char) * 9);
	offset += sizeof(char) * 9;
	memcpy(stream + offset, registros->ebx, sizeof(char) * 9);
	offset += sizeof(char) * 9;
	memcpy(stream + offset, registros->ecx, sizeof(char) * 9);
	offset += sizeof(char) * 9;
	memcpy(stream + offset, registros->edx, sizeof(char) * 9);
	offset += sizeof(char) * 9;
	memcpy(stream + offset, registros->rax, sizeof(char) * 17);
	offset += sizeof(char) * 17;
	memcpy(stream + offset, registros->rbx, sizeof(char) * 17);
	offset += sizeof(char) * 17;
	memcpy(stream + offset, registros->rcx, sizeof(char) * 17);
	offset += sizeof(char) * 17;
	memcpy(stream + offset, registros->rdx, sizeof(char) * 17);
}

void crear_header(void* a_enviar, t_buffer* buffer, int lineas){
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

void serializar_contexto(int socket, t_contexto* contexto){
	//Creo el buffer a utilizar para las instrucciones
	t_buffer* buffer = malloc(sizeof(t_buffer));
	//t_instruc* instrucciones = malloc(sizeof(t_instruc));

	//Leo la lista de instrucciones para sumar el tamaño de toda la lista
	buffer->size = calcular_tam_registros(contexto->registros);
	buffer->size = buffer->size + calcular_tam_instrucciones(contexto->instrucciones);

	//Asigno memoria para el stream del tamaño de mi lista
	void* stream = malloc(buffer->size);

	//Leo toda la lista para copiar los valores en memoria
	copiar_contexto(stream,contexto->instrucciones,contexto->registros);

	//Añado el stream a mi buffers
	buffer->stream = stream;

	void* a_enviar = malloc(buffer->size + sizeof(uint32_t) * 3);

	crear_header(a_enviar,buffer,list_size(contexto->instrucciones));
	//Creo un paquete y le asigno los valores iniciales

	//Envio todo el stream al servidor
	send(socket, a_enviar, buffer->size + sizeof(uint32_t) + sizeof(uint32_t) + sizeof(uint32_t), 0);

	//Libero memoria que ya no voy a utilizar
	free(a_enviar);
}

void liberar_conexion(int socket_cliente)
{
	close(socket_cliente);
}
