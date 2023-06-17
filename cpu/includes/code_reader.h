#ifndef CODE_READER_H_
#define CODE_READER_H_

#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include<commons/log.h>

#include"../../shared/includes/tad.h"
#include"utils.h"
#include"client_utils.h"
#include"server_utils.h"

int leer_instruccion(t_contexto *contexto, t_instruc *instruccion);
int ejecutar_set(char *param1, char *param2);
int ejecutar_yield(t_contexto *contexto);
int ejecutar_exit(t_contexto *contexto);
void cambiar_registro(char *registro, char *valor);
char* seleccionar_registro(char *param);
void log_instruccion(char *instruc, char *params);
int ejecutar_syscall(t_contexto *contexto, t_instruc *instruccion, contexto_estado_t estado, int cant_params);
int ejecutar_mov_in(t_contexto *contexto, t_instruc *instruccion);
char* esperar_valor(int memoria_connection);

extern t_log *logger;
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
extern char *contexto_param;
extern uint32_t contexto_estado;
extern t_contexto *contexto;

extern int memoria_connection;

#endif /* CODE_READER_H_ */
