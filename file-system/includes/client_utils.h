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

typedef enum
{
	MENSAJE,
	PAQUETE
}op_code;

extern t_log* logger;

int crear_conexion(char* ip, char* puerto);
void liberar_conexion(int socket_cliente);
int handshake_cliente(int socket_cliente, uint8_t tipo_cliente, uint8_t tipo_servidor);
void serializar_respuesta_file_kernel(int socket_cliente, t_resp_file respuesta);
void crear_header(void* a_enviar, t_buffer* buffer, int lineas, uint32_t codigo);
void serializar_instruccion_memoria(int socket,t_instruc_mem* instruccion);

#endif
