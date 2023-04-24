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

extern char* memoria_ip;
extern char* memoria_port;
extern char* file_system_ip;
extern char* file_system_port;
extern char* cpu_ip;
extern char* cpu_port;

extern t_log* logger;

extern char* server_port;

extern char* algoritmo_planificacion;
extern char* estimacion_inicial;
extern char* hrrn_alfa;

extern t_config* config;

int initial_setup(void);
void failed_initial_setup(char* key);

#endif /* INITIAL_SETUP_H_ */
