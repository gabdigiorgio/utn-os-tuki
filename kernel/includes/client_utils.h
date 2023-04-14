#ifndef CLIENT_UTILS_H_
#define CLIENT_UTILS_H_

#include<stdio.h>
#include<stdlib.h>
#include<signal.h>
#include<unistd.h>
#include<sys/socket.h>
#include<netdb.h>
#include<string.h>
#include<commons/log.h>

#include "../../shared/includes/tad.h"

int crear_conexion(char* ip, char* puerto);
void liberar_conexion(int socket_cliente);
char* handshake(int socket_cliente, uint8_t tipo_cliente);

#endif /* CLIENT_UTILS_H_ */
