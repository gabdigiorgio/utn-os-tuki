#ifndef UTILS_H_
#define UTILS_H_

#include<stdio.h>
#include<stdlib.h>
#include<sys/socket.h>
#include<unistd.h>
#include<netdb.h>
#include<string.h>
#include<assert.h>
#include<commons/log.h>
#include<commons/config.h>
#include "../../shared/includes/tad.h"

void imprimir_datos(void* memoria, int tam_memoria);
t_config* iniciar_config(char * path_config);
t_log* iniciar_logger(void);

extern t_config* config;
extern t_log* logger;
extern int tam_linea;

#endif /* UTILS_H_ */
