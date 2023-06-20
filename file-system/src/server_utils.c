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
	int socket_cliente = accept(socket_servidor, NULL, NULL);
	log_info(logger, "Se conecto el Kernel");

	return socket_cliente;
}
char* handshake(int socket_cliente){
	char* message = "";
	uint8_t handshake;
	bool kernel_conectada=NULL;
	uint8_t resultOk = 1;
	uint8_t resultError = -1;

	recv(socket_cliente, &handshake, sizeof(uint8_t), MSG_WAITALL); //recive el mensaje
	if(handshake == 2){
		kernel_conectada = 0;
	}
	if(handshake > 0){
		switch(handshake) {
			case 2:
				if(kernel_conectada == 0){
					send(socket_cliente, &resultOk, sizeof(uint8_t), NULL);
					message = "Handshake de Kernel recibido correctamente";
						}
				else {
					send(socket_cliente,&resultError,sizeof(uint8_t),NULL);
					message = "Error al intentar handshake";
				}
				break;
		}


	}
	return message;
}

void deserializar_header(t_paquete* paquete, int socket_cliente){
	recv(socket_cliente, &(paquete->codigo_operacion), sizeof(uint32_t), MSG_WAITALL);
	recv(socket_cliente, &(paquete->lineas), sizeof(uint32_t), MSG_WAITALL);
	recv(socket_cliente, &(paquete->buffer->size), sizeof(uint32_t), MSG_WAITALL);
	paquete->buffer->stream = malloc(paquete->buffer->size);
	recv(socket_cliente, paquete->buffer->stream, paquete->buffer->size, MSG_WAITALL);
}

void deserializar_instruccion_file(t_instruc_file* instruccion, t_buffer* buffer, int lineas){
	void* stream = buffer->stream;

	memcpy(&(instruccion->estado), stream, sizeof(contexto_estado_t));
	stream += sizeof(contexto_estado_t);

	memcpy(&(instruccion->pid), stream, sizeof(uint32_t));
	stream += sizeof(uint32_t);

	memcpy(&(instruccion->param1_length), stream, sizeof(uint32_t));
	stream += sizeof(uint32_t);
	instruccion->param1 = realloc(instruccion->param1,instruccion->param1_length);
	memcpy(instruccion->param1, stream, instruccion->param1_length);
	stream += instruccion->param1_length;
	memcpy(&(instruccion->param2_length), stream, sizeof(uint32_t));
	stream += sizeof(uint32_t);
	instruccion->param2 = realloc(instruccion->param2,instruccion->param2_length);
	memcpy(instruccion->param2, stream, instruccion->param2_length);
	stream += instruccion->param2_length;

	memcpy(&(instruccion->param3_length), stream, sizeof(uint32_t));
	stream += sizeof(uint32_t);
	instruccion->param3 = realloc(instruccion->param3,instruccion->param3_length);
	memcpy(instruccion->param3, stream, instruccion->param3_length);
	stream += instruccion->param3_length;
}

void deserializar_instruccion_memoria(t_instruc_mem* instruccion, t_buffer* buffer, int lineas){
	void* stream = buffer->stream;

	memcpy(&(instruccion->estado), stream, sizeof(contexto_estado_t));
	stream += sizeof(contexto_estado_t);

	memcpy(&(instruccion->pid), stream, sizeof(uint32_t));
	stream += sizeof(uint32_t);

	memcpy(&(instruccion->param1_length), stream, sizeof(uint32_t));
	stream += sizeof(uint32_t);
	instruccion->param1 = realloc(instruccion->param1,instruccion->param1_length);
	memcpy(instruccion->param1, stream, instruccion->param1_length);
	stream += instruccion->param1_length;
	memcpy(&(instruccion->param2_length), stream, sizeof(uint32_t));
	stream += sizeof(uint32_t);
	instruccion->param2 = realloc(instruccion->param2,instruccion->param2_length);
	memcpy(instruccion->param2, stream, instruccion->param2_length);
	stream += instruccion->param2_length;

	memcpy(&(instruccion->param3_length), stream, sizeof(uint32_t));
	stream += sizeof(uint32_t);
	instruccion->param3 = realloc(instruccion->param3,instruccion->param3_length);
	memcpy(instruccion->param3, stream, instruccion->param3_length);
	stream += instruccion->param3_length;
}

char* esperar_valor(int memoria_connection)
{
	t_paquete *paquete = malloc(sizeof(t_paquete));
	paquete->buffer = malloc(sizeof(t_buffer));
	deserializar_header(paquete, memoria_connection);

	switch (paquete->codigo_operacion)
	{
	case 1:
		t_instruc_mem *nueva_instruccion = inicializar_instruc_mem();
		deserializar_instruccion_memoria(nueva_instruccion, paquete->buffer, paquete->lineas);
		return nueva_instruccion->param2;
		break;
	default:
		log_error(logger, "Fallo respuesta file system a CPU");
		return NULL;
		break;
	}

	free(paquete->buffer->stream);
	free(paquete->buffer);
	free(paquete);
}
