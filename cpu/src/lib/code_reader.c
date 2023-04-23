#include "../../includes/code_reader.h"

void leer_instruccion(t_instruc* instruccion){
	if((strcmp(instruccion->instruct,"SET"))==0) return ejecutar_set(instruccion->param1,instruccion->param2);
	if((strcmp(instruccion->instruct,"YIELD"))==0) return ejecutar_yield();
	if((strcmp(instruccion->instruct,"EXIT"))==0) return ejecutar_exit();
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
}
