/*
 * utils.h
 *
 *  Created on: Apr 3, 2023
 *      Author: utnso
 */

#ifndef UTILS_H_
#define UTILS_H_

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

#define PUERTO "4444"

typedef enum
{
	MENSAJE,
	PAQUETE
}op_code;

extern t_log* logger;

void* recibir_buffer(int*, int);
t_config* iniciar_config(void);
int iniciar_servidor(void);
int esperar_cliente(int);
t_list* recibir_paquete(int);
void recibir_mensaje(int);
int recibir_operacion(int);
char* handshake(int);

#endif /* UTILS_H_ */
