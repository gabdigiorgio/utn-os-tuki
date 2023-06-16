/*
 * utils.h
 *
 *  Created on: Apr 3, 2023
 *      Author: utnso
 */

#ifndef SERVER_UTILS_H_
#define SERVER_UTILS_H_

#include<stdio.h>
#include<stdlib.h>
#include<sys/socket.h>
#include<unistd.h>
#include<netdb.h>
#include<commons/log.h>
#include<commons/collections/list.h>
#include<commons/config.h>
#include<string.h>
#include<assert.h>
#include "../../shared/includes/tad.h"
#include "utils.h"

extern t_log* logger;

int iniciar_servidor(char* puerto);
int esperar_cliente(int);
void deserializar_header(t_paquete* paquete, int socket);
void deserializar_instrucciones(t_list* lista, t_buffer* buffer, int lineas);
void deserializar_contexto(t_contexto* contexto, t_buffer* buffer, int lineas);
void deserializar_tabla_segmentos(t_lista_mutex* lista_tablas, t_buffer* buffer, int lineas);
t_resp_mem deserializar_respuesta_memoria(t_buffer* buffer);

#endif /* SERVER_UTILS_H_ */
