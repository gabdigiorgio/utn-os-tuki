/*
 * initial_setup.h
 *
 *  Created on: Apr 6, 2023
 *      Author: utnso
 */

#ifndef INITIAL_SETUP_H_
#define INITIAL_SETUP_H_

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <commons/log.h>
#include <commons/string.h>
#include <commons/config.h>

int initial_setup(void);
void failed_initial_setup(char* key);


extern char* server_port;
extern int tam_memoria;
extern int tam_segmento_0;
extern int cant_segmentos;
extern int retardo_memoria;
extern int retardo_compactacion;
extern char* algoritmo_asignacion;

extern t_config* config;
extern t_log* logger;

#endif /* INITIAL_SETUP_H_ */
