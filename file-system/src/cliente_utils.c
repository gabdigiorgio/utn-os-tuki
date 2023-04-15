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

int handshake_cliente(int socket_cliente, uint8_t tipo_cliente, uint8_t tipo_servidor){ //char* handshake (int socket_cliente, uint8_t tipo_cliente)

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

void liberar_conexion(int socket_cliente)
{
	close(socket_cliente);
}
