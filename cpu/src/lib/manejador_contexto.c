#include "../../includes/manejador_contexto.h"

int ejecutar_contexto(t_contexto* contexto, int lineas){
	t_list* instruc_lista;

	ip = contexto->registros->ip;

	//log_info(logger, "El IP esta en %d", contexto->registros->ip);

	copiar_string(contexto->registros->ax,ax);
	copiar_string(contexto->registros->bx,bx);
	copiar_string(contexto->registros->cx,cx);
	copiar_string(contexto->registros->dx,dx);
	copiar_string(contexto->registros->eax,eax);
	copiar_string(contexto->registros->ebx,ebx);
	copiar_string(contexto->registros->ecx,ecx);
	copiar_string(contexto->registros->edx,edx);
	copiar_string(contexto->registros->rax,rax);
	copiar_string(contexto->registros->rbx,rbx);
	copiar_string(contexto->registros->rcx,rcx);
	copiar_string(contexto->registros->rdx,rdx);

	instruc_lista = contexto->instrucciones;

	list_sort(instruc_lista,(void*)sort_list);

	ejecutar_instrucciones(contexto, instruc_lista, lineas);

	return 0;
}

void armar_contexto(t_contexto* contexto){
	contexto->registros->ip = ip;


	copiar_string(ax,contexto->registros->ax);
	copiar_string(bx,contexto->registros->bx);
	copiar_string(cx,contexto->registros->cx);
	copiar_string(dx,contexto->registros->dx);
	copiar_string(eax,contexto->registros->eax);
	copiar_string(ebx,contexto->registros->ebx);
	copiar_string(ecx,contexto->registros->ecx);
	copiar_string(edx,contexto->registros->edx);
	copiar_string(rax,contexto->registros->rax);
	copiar_string(rbx,contexto->registros->rbx);
	copiar_string(rcx,contexto->registros->rcx);
	copiar_string(rdx,contexto->registros->rdx);

	contexto->estado = contexto_estado;

}

void ejecutar_instrucciones(t_contexto* contexto, t_list* lista, int lineas){
	int instruc_pointer = ip;
	int exit = 0;

	while(instruc_pointer < lineas && exit == 0){
		t_instruc* instruccion;
		instruccion = list_get(lista, instruc_pointer);
		exit = leer_instruccion(contexto, instruccion);
		instruc_pointer++;
		sleep(retardo_instruc);
	}

	ip = instruc_pointer;
}
