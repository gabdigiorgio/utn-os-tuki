#ifndef HANDLER_FILE_H_
#define HANDLER_FILE_H_

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <dirent.h>
#include <sys/types.h>
#include <libgen.h>
#include <math.h>
#include <commons/log.h>
#include <commons/string.h>
#include <commons/config.h>
#include <commons/collections/list.h>
#include <commons/collections/dictionary.h>
#include "utils.h"

extern fcb_list_t* lista_global_fcb;
extern t_log* logger;
extern int fcb_id;
extern int tamanio_de_bloque;
extern void* memoria_file_system;
extern int retardo_acceso_bloque;

uint32_t leer_int(int offset, int size);
void* leer_dato(int offset, int size);
void* leer_datos(t_list* lista_offsets);

#endif /* HANDLER_FILE_H_ */
