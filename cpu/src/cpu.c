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

	while (salir != 1) {
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
		int lineas = list_size(instruc_lista);
				t_instruc* instrucciones = malloc(sizeof(t_instruc));

		/* for(int i = 0; i < lineas; i++){
					instrucciones = list_get(instruc_lista, i);
					log_info(logger,"--------------------");
					log_info(logger,instrucciones->instruct);
					if(strcmp(instrucciones->param1,"")) log_info(logger,instrucciones->param1);
					if(strcmp(instrucciones->param2,"")) log_info(logger,instrucciones->param2);
					if(strcmp(instrucciones->param3,"")) log_info(logger,instrucciones->param3);
					log_info(logger,"--------------------");
				} */

		salir = 1;
	}

	//list_iterate(instruc_lista,ejecutar_instruccion);
	//int connection_fd = esperar_cliente(server_connection);
	prueba_dps_borrar();
	//log_info(logger,handshake(connection_fd));
	t_list* lista;

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

void prueba_dps_borrar(){
	t_instruc* instruccion = malloc(sizeof(t_instruc));

	instruccion->instruct = "SET";
	instruccion->param1 = "AX";
	instruccion->param2 = "0090";

	leer_instruccion(instruccion);

	t_instruc* instruccion2 = malloc(sizeof(t_instruc));

	instruccion2->instruct = "YIELD";
	leer_instruccion(instruccion2);

	t_instruc* instruccion3 = malloc(sizeof(t_instruc));

	instruccion3->instruct = "SET";
	instruccion3->param1 = "RAX";
	instruccion3->param2 = "AAAA";

	leer_instruccion(instruccion3);

	t_instruc* instruccion4 = malloc(sizeof(t_instruc));

	instruccion4->instruct = "EXIT";
	leer_instruccion(instruccion4);


}



