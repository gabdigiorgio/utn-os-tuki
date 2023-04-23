/*
 ============================================================================
 Name        : cpu.c
 Author      : 
 Version     :
 Copyright   : Your copyright notice
 Description : Hello World in C, Ansi-style
 ============================================================================
 */

#include "../includes/cpu.h"

//Declaramos los registros de proposito general
uint16_t ip;
char ax[5];
char bx[5];
char cx[5];
char dx[5];
char eax[9];
char ebx[9];
char ecx[9];
char edx[9];
char rax[17];
char rbx[17];
char rcx[17];
char rdx[17];
t_list* instruc_lista;


int main(int argc, char *argv[]) {
	//Iniciamos tanto el log como el config
	logger = iniciar_logger();
	instruc_lista = list_create();

	 if (argc < 2) {
		 log_error(logger, "Falta parametro del path del archivo de configuracion");
		 return EXIT_FAILURE;
	 }

	config = iniciar_config(argv[1]);

	//Inicializamos las variables globales desde el config, que loggee errores o success si todo esta bien
	int exit_status = initial_setup();
	if (exit_status==EXIT_FAILURE){
		return EXIT_FAILURE;
	}

	if((memoria_connection = crear_conexion(memoria_ip,memoria_port)) == 0 || handshake_cliente(memoria_connection,1,4) == -1) {
		terminar_programa();
		return EXIT_FAILURE;
	}

	int server_connection = iniciar_servidor(server_port);

	log_info(logger, "Cpu lista para recibir al Kernel");
	int connection_fd = esperar_cliente(server_connection);
	log_info(logger,handshake(connection_fd));

	t_contexto* contexto = malloc(sizeof(t_contexto));

	contexto->instrucciones = list_create();

	int salir = 0;

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
				break;
			default:
				break;
		}
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
	}

	return EXIT_SUCCESS;
}
void iterator(char* value) {
	log_info(logger,"%s", value);
}
int registro_4b(char registro[5]){
	if(strlen(registro)!= 4){
		return EXIT_FAILURE;
	}
	//printf("%s",registro);
	return 0;
}

int registro_8b(char registro[9]){
	if(strlen(registro)!= 8){
		return EXIT_FAILURE;
	}
	//printf("%s",registro);
	return 0;
}
int registro_16b(char registro[16]){
	if(strlen(registro)!= 16){
		return EXIT_FAILURE;
	}
	//printf("%s",registro);
	return 0;
}

void terminar_programa()
{
	log_destroy(logger);
	config_destroy(config);
	liberar_conexion(memoria_connection);
}

static bool sort_list(t_instruc* instruc1, t_instruc* instruc2){
	return instruc1->nro < instruc2->nro;
}


