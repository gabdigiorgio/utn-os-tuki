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
#include "utils.h"

extern fcb_list_t* lista_global_fcb;
extern t_log* logger;
extern int fcb_id;

void inicializar_fcb_list();
int buscar_fcb(char* nombre_fcb);
fcb_t* get_fcb(char* nombre);
fcb_t* get_fcb_id(int id);
void modificar_fcb_id(int id,fcb_prop_t llave, uint32_t valor);
void modificar_fcb(fcb_t* fcb, fcb_prop_t llave, uint32_t valor);

#endif /* FCB_LIST_H_ */
