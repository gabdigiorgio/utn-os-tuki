#ifndef COMM_FILE_H_
#define COMM_FILE_H_

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
extern int file_system_connection;
extern t_log* logger;

void manejar_archivo(t_contexto* contexto, pcb_t* pcb);
void editar_archivo(t_contexto* contexto, pcb_t* pcb);
t_resp_file esperar_respuesta_file();


#endif /* COMM_FILE_H_ */
