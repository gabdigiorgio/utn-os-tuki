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
#include "utils.h"

int initial_setup(void);
void failed_initial_setup(char* key);

extern char* memoria_ip;
extern char* memoria_port;

extern char* server_port;

extern char* path_superbloque;
extern char* path_bitmap;
extern char* path_bloques;
extern char* path_fcb_folder;

extern int retardo_acceso_bloque;

extern t_config* config;

extern t_log* logger;

#endif /* INITIAL_SETUP_H_ */
