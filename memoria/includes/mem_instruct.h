/*
 * utils.h
 *
 *  Created on: Apr 3, 2023
 *      Author: utnso
 */

#ifndef MEM_INSTRUCT_H_
#define MEM_INSTRUCT_H_

#include<stdio.h>
#include<stdlib.h>
#include<sys/socket.h>
#include<unistd.h>
#include<netdb.h>
#include<commons/log.h>
#include<commons/collections/list.h>
#include<commons/config.h>
#include<stdbool.h>
#include<string.h>
#include<assert.h>
#include "../../shared/includes/tad.h"
#include "utils.h"

extern t_list *lista_de_tablas;

void allocate_segmento_0(int pid);

#endif /* MEM_INSTRUCT_H_ */
