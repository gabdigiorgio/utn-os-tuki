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
#include<commons/string.h>
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
uint32_t calcular_tam_instruc_mem(t_instruc_mem* instruccion);
void copiar_instruccion_memoria(void* stream, t_instruc_mem* instruccion);
void serializar_instruccion_memoria(int socket,t_instruc_mem* instruccion);
void serializar_memoria(int socket_cliente, void* memoria, int tam_memoria);
void serializar_instruccion_mov(int socket,t_instruc_mov* instruccion);
void generar_instruccion_mov(t_instruc_mov* instruccion_nueva,contexto_estado_t instruccion, uint32_t dir_fisica, uint32_t tamanio, char* valor);
uint32_t calcular_tam_instruc_mov(t_instruc_mov* instruccion);
t_instruc_mov* inicializar_instruc_mov();

#endif
