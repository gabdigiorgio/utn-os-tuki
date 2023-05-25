#ifndef COMM_THREAD_H_
#define COMM_THREAD_H_

#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include<commons/log.h>
#include<commons/collections/list.h>

#include "server_utils.h"
#include "code_reader.h"
#include "utils.h"
#include "../../shared/includes/tad.h"

void conexion_kernel(int server_connection);
int ejecutar_contexto(t_contexto* contexto, int lineas);
void armar_contexto(t_contexto* contexto);
void serializar_contexto(int socket_cliente, t_contexto* contexto);

extern t_log* logger;
extern uint16_t ip;
extern char ax[5];
extern char bx[5];
extern char cx[5];
extern char dx[5];
extern char eax[9];
extern char ebx[9];
extern char ecx[9];
extern char edx[9];
extern char rax[17];
extern char rbx[17];
extern char rcx[17];
extern char rdx[17];
extern t_contexto* contexto;


#endif /* COMM_THREAD_H_ */


