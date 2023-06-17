/*
 * utils.h
 *
 *  Created on: Apr 3, 2023
 *      Author: utnso
 */

#ifndef CLIENT_UTILS_H_
#define CLIENT_UTILS_H_

#include<stdio.h>
#include<stdlib.h>
#include<sys/socket.h>
#include<unistd.h>
#include<netdb.h>
#include<commons/log.h>
#include<commons/collections/list.h>
#include<commons/config.h>
#include<stdbool.h>
#include<string.h>
#include<assert.h>
#include "../../shared/includes/tad.h"


void crear_header(void* a_enviar, t_buffer* buffer, uint32_t lineas);
int calcular_tam_total(t_list* lista_tablas);
void copiar_tabla_segmentos(void* stream, t_list* lista_tablas);
void serializar_tabla_segmentos(int socket, t_list* lista_tablas);
void serializar_respuesta_memoria_kernel(int socket_cliente,t_resp_mem respuesta);
uint32_t calcular_tam_instruc_mem(t_instruc_mem* instruccion);
void copiar_instruccion_memoria(void* stream, t_instruc_mem* instruccion);
void serializar_instruccion_memoria(int socket,t_instruc_mem* instruccion);

#endif /* CLIENT_UTILS_H_ */
