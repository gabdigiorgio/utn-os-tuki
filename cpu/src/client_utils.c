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

int handshake_cliente(int socket_cliente, uint8_t tipo_cliente, uint8_t tipo_servidor){

	char* message = "";
	uint8_t handshake = tipo_cliente;
	uint8_t result = 0;

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
	recv(socket_cliente, &result, sizeof(uint8_t), MSG_WAITALL);

	if(result == 1){
		log_info(logger, "Se establecio correctamente la conexion");
	} else {
		log_error(logger, "Fallo al realizar el handshake, cerrando conexion");
		result = -1;
	}

	return result;
}

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

void copiar_contexto(void* stream, t_contexto* contexto){
	int lineas = list_size(contexto->instrucciones);
	int offset = 0;

	for(int i = 0; i < lineas; i++){
			t_instruc* instrucciones = list_get(contexto->instrucciones, i);

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

	memcpy(stream + offset, &contexto->registros->ip, sizeof(uint16_t));
	offset += sizeof(uint16_t);
	memcpy(stream + offset, contexto->registros->ax, sizeof(char) * 5);
	offset += sizeof(char) * 5;
	memcpy(stream + offset, contexto->registros->bx, sizeof(char) * 5);
	offset += sizeof(char) * 5;
	memcpy(stream + offset, contexto->registros->cx, sizeof(char) * 5);
	offset += sizeof(char) * 5;
	memcpy(stream + offset, contexto->registros->dx, sizeof(char) * 5);
	offset += sizeof(char) * 5;
	memcpy(stream + offset, contexto->registros->eax, sizeof(char) * 9);
	offset += sizeof(char) * 9;
	memcpy(stream + offset, contexto->registros->ebx, sizeof(char) * 9);
	offset += sizeof(char) * 9;
	memcpy(stream + offset, contexto->registros->ecx, sizeof(char) * 9);
	offset += sizeof(char) * 9;
	memcpy(stream + offset, contexto->registros->edx, sizeof(char) * 9);
	offset += sizeof(char) * 9;
	memcpy(stream + offset, contexto->registros->rax, sizeof(char) * 17);
	offset += sizeof(char) * 17;
	memcpy(stream + offset, contexto->registros->rbx, sizeof(char) * 17);
	offset += sizeof(char) * 17;
	memcpy(stream + offset, contexto->registros->rcx, sizeof(char) * 17);
	offset += sizeof(char) * 17;
	memcpy(stream + offset, contexto->registros->rdx, sizeof(char) * 17);
	offset += sizeof(char) * 17;
	memcpy(stream + offset, &contexto->pid, sizeof(uint32_t));
	offset += sizeof(uint32_t);
	memcpy(stream + offset, &contexto->estado, sizeof(contexto_estado_t));
	offset += sizeof(contexto_estado_t);

	memcpy(stream + offset, &contexto->param1_length, sizeof(uint32_t));
	offset += sizeof(uint32_t);
	memcpy(stream + offset, contexto->param1, contexto->param1_length);
	offset += contexto->param1_length;

	memcpy(stream + offset, &contexto->param2_length, sizeof(uint32_t));
	offset += sizeof(uint32_t);
	memcpy(stream + offset, contexto->param2, contexto->param2_length);
	offset += contexto->param2_length;

	memcpy(stream + offset, &contexto->param3_length, sizeof(uint32_t));
	offset += sizeof(uint32_t);
	memcpy(stream + offset, contexto->param3, contexto->param3_length);
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
	t_buffer* buffer = malloc(sizeof(*buffer));

	//Leo la lista de instrucciones para sumar el tamaño de toda la lista
	buffer->size = calcular_tam_registros(contexto->registros);
	buffer->size = buffer->size + calcular_tam_contexto(contexto);
	buffer->size = buffer->size + calcular_tam_instrucciones(contexto->instrucciones);

	//Asigno memoria para el stream del tamaño de mi lista
	void* stream = malloc(buffer->size);

	//Leo toda la lista para copiar los valores en memoria
	copiar_contexto(stream,contexto);

	//Añado el stream a mi buffers
	buffer->stream = stream;

	void* a_enviar = malloc(buffer->size + sizeof(uint32_t) * 3);

	crear_header(a_enviar,buffer,list_size(contexto->instrucciones));
	//Creo un paquete y le asigno los valores iniciales

	//Envio todo el stream al servidor
	send(socket, a_enviar, buffer->size + sizeof(uint32_t) + sizeof(uint32_t) + sizeof(uint32_t), 0);

	//Libero memoria que ya no voy a utilizar
	free(buffer->stream);
	free(buffer);
	free(a_enviar);
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

void copiar_instruccion_memoria(void* stream, t_instruc_mem* instruccion){
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

void serializar_instruccion_memoria(int socket,t_instruc_mem* instruccion)
{
	//Creo el buffer a utilizar para las instrucciones
		t_buffer* buffer = malloc(sizeof(t_buffer));

		buffer->size = calcular_tam_instruc_mem(instruccion);

		//Asigno memoria para el stream del tamaño de mi lista
		void* stream = malloc(buffer->size);

		//Leo toda la lista para copiar los valores en memoria
		copiar_instruccion_memoria(stream,instruccion);

		//Añado el stream a mi buffers
		buffer->stream = stream;

		void* a_enviar = malloc(buffer->size + sizeof(uint32_t) * 3);

		crear_header(a_enviar,buffer,0);

		//Envio todo el stream al servidor
		send(socket, a_enviar, buffer->size + sizeof(uint32_t) + sizeof(uint32_t) + sizeof(uint32_t), 0);

		//Libero memoria que ya no voy a utilizar
		free(buffer->stream);
		free(buffer);
		free(a_enviar);
}

void liberar_conexion(int socket_cliente)
{
	close(socket_cliente);
}
