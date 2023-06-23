#ifndef COMM_THREADKERNEL_H_
#define COMM_THREADKERNEL_H_

#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include<commons/log.h>
#include<commons/collections/list.h>

#include "server_utils.h"
#include "utils.h"
#include "datos.h"
#include "mem_instruct.h"
#include "algoritmos.h"
#include "client_utils.h"
#include "../../shared/includes/tad.h"

void conexion_kernel(int server_connection);

extern int exit_status;
extern t_log* logger;
extern uint16_t ip;
extern t_list* lista_de_tablas;
extern char* algoritmo_asignacion;
extern void* memoria;


#endif /* COMM_THREADKERNEL_H_ */
