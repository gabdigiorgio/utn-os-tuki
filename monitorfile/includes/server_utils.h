#ifndef SERVER_UTILS_H_
#define SERVER_UTILS_H_

#include<stdio.h>
#include<stdlib.h>
#include<sys/socket.h>
#include<unistd.h>
#include<netdb.h>
#include<string.h>
#include<assert.h>
#include "../../shared/includes/tad.h"

int iniciar_servidor(char* puerto);
int esperar_cliente(int);
void deserializar_header(t_paquete* paquete, int socket_cliente);
void deserializar_memoria(t_buffer* buffer, int lineas);

#endif /* SERVER_UTILS_H_ */
