#ifndef COMM_MEM_H_
#define COMM_MEM_H_

#include "../../shared/includes/tad.h"
#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include "semaphore.h"
#include "client_utils.h"
#include "server_utils.h"
#include "utils.h"
#include "long_term_planification.h"
#include "short_term_planification.h"

//extern  pcb_t* pcb;
extern int memoria_connection;
extern t_log* logger;
extern t_lista_mutex* lista_tabla_segmentos;
extern sem_t sem_compactacion;

void solicitar_tabla_segmentos();
t_resp_mem esperar_respuesta_memoria();
void create_segment(t_contexto* contexto, pcb_t* pcb);
void delete_segment(t_contexto* contexto, pcb_t* pcb);


#endif /* COMM_MEM_H_ */
