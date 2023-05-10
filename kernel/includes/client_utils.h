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

extern t_log* logger;

int crear_conexion(char* ip, char* puerto);
void liberar_conexion(int socket_cliente);
int handshake(int socket_cliente, uint8_t tipo_cliente, uint8_t tipo_servidor);
uint32_t calcular_tam_instrucciones(t_list* lista);
void copiar_contexto(void* stream, t_contexto* contexto);
void crear_header(void* a_enviar, t_buffer* buffer, int lineas);
void serializar_contexto(int socket, t_contexto* contexto);
uint32_t calcular_tam_registros(t_registros* registros);
uint32_t calcular_tam_contexto(t_contexto* contexto);

#endif /* CLIENT_UTILS_H_ */
