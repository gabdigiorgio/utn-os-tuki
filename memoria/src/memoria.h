/*
 * memoria.h
 *
 *  Created on: Apr 3, 2023
 *      Author: utnso
 */

#ifndef MEMORIA_H_
#define MEMORIA_H_

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <commons/log.h>
#include <pthread.h>
#include "utils.h"
#include "server_utils.h"

typedef struct
{
	int num_socket;
	int id_cliente;
} t_conexion;

void iterator(char* value);
char* initial_setup(void);
void thread_main(t_conexion *conexion);

#endif /* MEMORIA_H_ */
