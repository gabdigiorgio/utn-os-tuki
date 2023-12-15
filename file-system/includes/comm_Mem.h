#ifndef COMM_MEM_H_
#define COMM_MEM_H_

#include "../../shared/includes/tad.h"
#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include "client_utils.h"
#include "server_utils.h"
#include "utils.h"

//extern  pcb_t* pcb;
extern int memoria_connection;
extern t_log* logger;

void realizar_f_write(t_instruc_file* instruccion_file);
void realizar_f_read(t_instruc_file* instruccion_file);

#endif /* COMM_MEM_H_ */
