#ifndef COMM_MEM_H_
#define COMM_MEM_H_

#include "../../shared/includes/tad.h"
#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include "client_utils.h"
#include "server_utils.h"
#include "utils.h"
#include "long_term_planification.h"
#include "short_term_planification.h"

//extern  pcb_t* pcb;
extern int memoria_connection;
extern t_log* logger;
extern t_lista_mutex* lista_tabla_segmentos;

void solicitar_tabla_segmentos();


#endif /* COMM_MEM_H_ */
