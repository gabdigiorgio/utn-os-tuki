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
#include <math.h>
#include <sys/mman.h>
#include <fcntl.h>
#include <commons/bitarray.h>
#include <commons/log.h>
#include <commons/string.h>
#include <commons/config.h>
#include "utils.h"

int initial_setup(void);
int leer_superbloque_config();
int crear_bitmap();
void failed_setup(char* key);

extern char* memoria_ip;
extern char* memoria_port;

extern char* server_port;

extern char* path_superbloque;
extern char* path_bitmap;
extern char* path_bloques;
extern char* path_fcb_folder;

extern int retardo_acceso_bloque;

 // Variables de SuperBloque
extern int tamanio_de_bloque;
extern int cantidad_de_bloques;

extern t_config* config;
extern t_config* superbloque_config;

// Bitmap
extern t_bitarray *bitmap;

extern t_log* logger;

#endif /* INITIAL_SETUP_H_ */
