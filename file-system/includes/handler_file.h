#ifndef HANDLER_FILE_H_
#define HANDLER_FILE_H_

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <dirent.h>
#include <sys/types.h>
#include <libgen.h>
#include <math.h>
#include <unistd.h>
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
void leer_bloques_indirectos(int id_fcb,t_list* lista_de_bloques, int offset_inicial, int offset_final);
uint32_t _leer_int(int offset, int size);
void escribir_dato(void* dato, int offset, int size);
void escribir_int(uint32_t dato,int offset);
void _escribir_int(uint32_t dato, int offset);
void escribir_datos(void* datos, t_list* lista_offsets);
void escribir_bloques_indirectos(t_list* lista_bloques, int indice_inicial);

#endif /* HANDLER_FILE_H_ */
