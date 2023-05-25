#ifndef MANEJADOR_CONTEXTO_H_
#define MANEJADOR_CONTEXTO_H_

#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include<commons/log.h>
#include<commons/collections/list.h>

#include "server_utils.h"
#include "code_reader.h"
#include "utils.h"
#include "../../shared/includes/tad.h"

int ejecutar_contexto(t_contexto* contexto, int lineas);
void armar_contexto();
int leer_instruccion(t_contexto* contexto, t_instruc* instruccion);

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
extern char* contexto_param;
extern uint32_t contexto_estado;
extern t_contexto* contexto;

extern int retardo_instruc;


#endif /* MANEJADOR_CONTEXTO_H_ */
