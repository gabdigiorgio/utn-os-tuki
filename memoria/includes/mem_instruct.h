#ifndef MEM_INSTRUCT_H_
#define MEM_INSTRUCT_H_

#include<stdio.h>
#include<commons/log.h>
#include<commons/collections/list.h>
#include "../../shared/includes/tad.h"

extern t_list *lista_de_tablas;
extern t_log* logger;

void allocate_segmento_0(int pid);

#endif /* MEM_INSTRUCT_H_ */
