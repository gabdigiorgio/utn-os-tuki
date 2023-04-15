#include "../includes/server_utils.h"

int iniciar_servidor(char* puerto)
{
	// Quitar esta línea cuando hayamos terminado de implementar la funcion
	//assert(!"no implementado!");

	int socket_servidor;
	struct addrinfo hints, *servinfo, *p;

	memset(&hints, 0, sizeof(hints));
	hints.ai_family = AF_UNSPEC;
	hints.ai_socktype = SOCK_STREAM;
	hints.ai_flags = AI_PASSIVE;

	getaddrinfo(NULL, puerto, &hints, &servinfo);

	// Creamos el socket de escucha del servidor
	socket_servidor = socket(servinfo->ai_family,servinfo->ai_socktype,servinfo->ai_protocol);

	int enable_reuse = 1;
	setsockopt(socket_servidor, SOL_SOCKET, SO_REUSEADDR, &enable_reuse, sizeof(int));

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

char* handshake(int socket_cliente){
	char* message = "";
	uint8_t handshake;
	uint8_t resultOk = 1;
	uint8_t resultError = -1;

	recv(socket_cliente, &handshake, sizeof(uint8_t), MSG_WAITALL); //recive el mensaje

	if(handshake > 0){
		switch(handshake) {

			case 1:
				if(cpu_conectada == false){
					send(socket_cliente, &resultOk, sizeof(uint8_t), NULL);
					message = "Handshake de CPU recibido correctamente";
					cpu_conectada = true;
				}
				else {
					send(socket_cliente,&resultError,sizeof(uint8_t),NULL);
					message = "Error al intentar handshake";
				}
				break;

			case 2:
				if(kernel_conectado == false){
					send(socket_cliente, &resultOk, sizeof(uint8_t), NULL);
					message = "Handshake de Kernel recibido correctamente";
					kernel_conectado = true;
						}
				else {
					send(socket_cliente,&resultError,sizeof(uint8_t),NULL);
					message = "Error al intentar handshake";
				}
				break;
			case 3:
				if(fileSystem_conectado == false){
					send(socket_cliente, &resultOk, sizeof(uint8_t), NULL);
					message = "Handshake de File System recibido correctamente";
					fileSystem_conectado = true;
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

void liberar_conexion(int socket_servidor)
{
	close(socket_servidor);
}
