#ifndef FCB_LIST_H_
#define FCB_LIST_H_

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <dirent.h>
#include <sys/types.h>
#include <libgen.h>
#include <commons/log.h>
#include <commons/string.h>
#include <commons/config.h>
#include <commons/collections/list.h>
#include <commons/collections/dictionary.h>
#include "utils.h"

extern fcb_list_t* lista_global_fcb;
extern t_log* logger;
extern int fcb_id;

void inicializar_fcb_list();
int buscar_fcb(char* nombre_fcb);
fcb_t* _get_fcb(char* nombre);
fcb_t* _get_fcb_id(int id);
int modificar_fcb(int id,fcb_prop_t llave, uint32_t valor);
int _modificar_fcb(fcb_t* fcb, fcb_prop_t llave, uint32_t valor);
uint32_t valor_fcb(int id,fcb_prop_t llave);
uint32_t _valor_fcb(fcb_t* fcb, fcb_prop_t llave);
int nombre_fcb(int id,char* nombre);
int crear_fcb(char* nombre_fcb);
int buscar_fcb_id(int id);
int borrar_fcb(int id);

int obtener_cantidad_de_bloques(int id_fcb);
t_list* obtener_lista_de_bloques(int id_fcb);
void asignar_bloques(int id_fcb, int nuevo_tamanio);
void desasignar_bloques(int id_fcb, int nuevo_tamanio);

#endif /* FCB_LIST_H_ */
