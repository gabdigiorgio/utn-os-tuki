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
	log_info(logger, "Se conecto el Kernel");

	return socket_cliente;
}

void deserializar_header(t_paquete* paquete, int socket){
	recv(socket, &(paquete->codigo_operacion), sizeof(uint32_t), MSG_WAITALL);
	recv(socket, &(paquete->lineas), sizeof(uint32_t), MSG_WAITALL);
	recv(socket, &(paquete->buffer->size), sizeof(uint32_t), MSG_WAITALL);
	paquete->buffer->stream = malloc(paquete->buffer->size);
	recv(socket, paquete->buffer->stream, paquete->buffer->size, MSG_WAITALL);
}

t_contexto* deserializar_contexto(t_buffer* buffer, int lineas, t_contexto* contexto){
	uint32_t pid;
	contexto_estado_t estado;

	void* stream = buffer->stream;

	for(int i=0; i<lineas; i++){
			t_instruc* instrucciones = malloc(sizeof(t_instruc));

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
	memcpy(&(pid), stream, sizeof(uint32_t));
	stream += sizeof(uint32_t);
	memcpy(&(estado), stream, sizeof(contexto_estado_t));
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
	contexto->pid = pid;
	contexto->estado = estado;

	return contexto;
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

