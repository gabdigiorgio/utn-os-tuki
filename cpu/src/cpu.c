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


int main(int argc, char *argv[]) {
	//Iniciamos tanto el log como el config
	logger = iniciar_logger();

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

	if((memoria_connection = crear_conexion(memoria_ip,memoria_port)) != 0) log_info(logger, "Conexion establecida con la memoria");

	log_info(logger, handshake(memoria_connection,1)); //handshake(memoria_connection,1)

	int server_connection = iniciar_servidor(server_port);

	log_info(logger, "Cpu lista para recibir al Kernel");
	int connection_fd = esperar_cliente(server_connection);
	t_list* lista;
	while (1) {
			int cod_op = recibir_operacion(connection_fd);
			switch (cod_op) {
			case MENSAJE:
				recibir_mensaje(connection_fd);
				break;
			case PAQUETE:
				lista = recibir_paquete(connection_fd);
				log_info(logger, "Me llegaron los siguientes valores:\n");
				list_iterate(lista, (void*) iterator);
				break;
			case -1:
				log_error(logger, "el cliente se desconecto. Terminando servidor");
				return EXIT_FAILURE;
			default:
				log_warning(logger,"Operacion desconocida. No quieras meter la pata");
				break;
			}
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



