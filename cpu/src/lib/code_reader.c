#include "../../includes/code_reader.h"

int leer_instruccion(t_contexto* contexto, t_instruc* instruccion){
	//Genericas
	if((strcmp(instruccion->instruct,"SET"))==0) return ejecutar_set(instruccion->param1,instruccion->param2);
	if((strcmp(instruccion->instruct,"YIELD"))==0) return ejecutar_yield(contexto);
	if((strcmp(instruccion->instruct,"EXIT"))==0) return ejecutar_exit(contexto);
	//IO
	if((strcmp(instruccion->instruct,"I/O"))==0) return ejecutar_syscall(contexto,instruccion,IO,1);
	//Semasforos
	if((strcmp(instruccion->instruct,"WAIT"))==0) return ejecutar_syscall(contexto,instruccion,WAIT,1);
	if((strcmp(instruccion->instruct,"SIGNAL"))==0) return ejecutar_syscall(contexto,instruccion,SIGNAL,1);
	//Memoria
	if((strcmp(instruccion->instruct,"CREATE_SEGMENT"))==0) return ejecutar_syscall(contexto,instruccion,CREATE_SEGMENT,2);
	if((strcmp(instruccion->instruct,"DELETE_SEGMENT"))==0) return ejecutar_syscall(contexto,instruccion,DELETE_SEGMENT,1);
	if((strcmp(instruccion->instruct,"MOV_IN"))==0) return ejecutar_mov_in(contexto, instruccion);
	if((strcmp(instruccion->instruct,"MOV_OUT"))==0) return ejecutar_mov_out(contexto,instruccion);
	if((strcmp(instruccion->instruct,"PRINT_SEGMENTS"))==0) return ejecutar_syscall(contexto,instruccion,PRINT_SEGMENTS,1);
	//File System
	if((strcmp(instruccion->instruct,"F_OPEN"))==0) return ejecutar_syscall(contexto,instruccion,F_OPEN,1);
	if((strcmp(instruccion->instruct,"F_CLOSE"))==0) return ejecutar_syscall(contexto,instruccion,F_CLOSE,1);
	if((strcmp(instruccion->instruct,"F_DELETE"))==0) return ejecutar_syscall(contexto,instruccion,F_DELETE,1);
	if((strcmp(instruccion->instruct,"F_SEEK"))==0) return ejecutar_syscall(contexto,instruccion,F_SEEK,2);
	if((strcmp(instruccion->instruct,"F_READ"))==0) return ejecutar_syscall(contexto,instruccion,F_READ,3);
	if((strcmp(instruccion->instruct,"F_WRITE"))==0) return ejecutar_syscall(contexto,instruccion,F_WRITE,3);
	if((strcmp(instruccion->instruct,"F_TRUNCATE"))==0) return ejecutar_syscall(contexto,instruccion,F_TRUNCATE,2);

	log_error(logger,"Instruccion desconocida [%s]",instruccion->instruct);
	return ejecutar_exit(contexto);
}

char* seleccionar_registro(char* param){
	if((strcmp(param,"AX"))==0) return ax;
	if((strcmp(param,"BX"))==0) return bx;
	if((strcmp(param,"CX"))==0) return cx;
	if((strcmp(param,"DX"))==0) return dx;
	if((strcmp(param,"EAX"))==0) return eax;
	if((strcmp(param,"EBX"))==0) return ebx;
	if((strcmp(param,"ECX"))==0) return ecx;
	if((strcmp(param,"EDX"))==0) return edx;
	if((strcmp(param,"RAX"))==0) return rax;
	if((strcmp(param,"RBX"))==0) return rbx;
	if((strcmp(param,"RCX"))==0) return rcx;
	if((strcmp(param,"RDX"))==0) return rdx;

	return "";
}
