#ifndef INITIAL_SETUP_H_
#define INITIAL_SETUP_H_

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <commons/config.h>
#include "server_utils.h"
#include "utils.h"

extern t_config* config;
extern t_log* logger;
extern char* server_port;
extern int tam_linea;

int initial_setup();
void failed_initial_setup(char* key);

#endif /* INITIAL_SETUP_H_ */
