#include "../includes/comm_thread.h"

void conexion_kernel(int server_connection){

	log_info(logger, "Cpu lista para recibir al Kernel");
	int connection_fd = esperar_cliente(server_connection);
	log_info(logger,handshake(connection_fd));

	while (1) {
		//Reservo memoria para el paquete
		t_paquete* paquete = malloc(sizeof(t_paquete));
		paquete->buffer = malloc(sizeof(t_buffer));

		//Recivo el header del paquete + el stream de datos
		deserializar_header(paquete, connection_fd);

		//Reviso el header para saber de que paquete se trata y deserealizo acorde
		switch(paquete->codigo_operacion){
			case 1:
				contexto = deserializar_contexto(paquete->buffer, paquete->lineas);
				int result = ejecutar_contexto();
				armar_contexto();
				serializar_contexto(connection_fd,contexto);
				break;
			default:
				break;
		}
	}
}

int ejecutar_contexto(){
	t_list* instruc_lista = list_create();

	ip = contexto->registros->ip;
	strcpy(contexto->registros->ax,ax);
	strcpy(contexto->registros->bx,bx);
	strcpy(contexto->registros->cx,cx);
	strcpy(contexto->registros->dx,dx);
	strcpy(contexto->registros->eax,eax);
	strcpy(contexto->registros->ebx,ebx);
	strcpy(contexto->registros->ecx,ecx);
	strcpy(contexto->registros->edx,edx);
	strcpy(contexto->registros->rax,rax);
	strcpy(contexto->registros->rbx,rbx);
	strcpy(contexto->registros->rcx,rcx);
	strcpy(contexto->registros->rdx,rdx);

	instruc_lista = contexto->instrucciones;

	list_sort(instruc_lista,(void*)sort_list);

	list_iterate(instruc_lista,ejecutar_instruccion);

	return 0;
}

void armar_contexto(){
	contexto->registros->ip = ip;
	strcpy(ax,contexto->registros->ax);
	strcpy(bx,contexto->registros->bx);
	strcpy(cx,contexto->registros->cx);
	strcpy(dx,contexto->registros->dx);
	strcpy(eax,contexto->registros->eax);
	strcpy(ebx,contexto->registros->ebx);
	strcpy(ecx,contexto->registros->ecx);
	strcpy(edx,contexto->registros->edx);
	strcpy(rax,contexto->registros->rax);
	strcpy(rbx,contexto->registros->rbx);
	strcpy(rcx,contexto->registros->rcx);
	strcpy(rdx,contexto->registros->rdx);
}
