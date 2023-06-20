#ifndef SERVER_UTILS_H_
#define SERVER_UTILS_H_

#include<stdio.h>
#include<stdlib.h>
#include<sys/socket.h>
#include<unistd.h>
#include<netdb.h>
#include<commons/log.h>
#include<commons/collections/list.h>
#include<string.h>
#include<assert.h>
#include "../../shared/includes/tad.h"
#include "utils.h"

extern t_log* logger;

void* recibir_buffer(int*, int);

int iniciar_servidor(char*);
int esperar_cliente(int);
char* handshake(int);
void deserializar_header(t_paquete* paquete, int socket_cliente);
void deserializar_instruccion_file(t_instruc_file* instruccion, t_buffer* buffer, int lineas);
char* esperar_valor(int memoria_connection);


#endif
