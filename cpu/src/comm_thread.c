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
				log_info(logger,contexto->registros->ax);
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

	list_iterate(instruc_lista,(void*)ejecutar_instruccion);

	return 0;
}

void armar_contexto(){
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
}
