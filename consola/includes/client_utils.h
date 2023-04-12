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

#include "tad.h"

int crear_conexion(char* ip, char* puerto);
void liberar_conexion(int socket_cliente);
void serializar_instrucciones(int socket,t_list* lista);

#endif
