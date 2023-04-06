/*
 ============================================================================
 Name        : kernel.c
 Author      : 
 Version     :
 Copyright   : Your copyright notice
 Description : Hello World in C, Ansi-style
 ============================================================================
 */


#include "../includes/kernel.h"


t_log* logger;

int server_connection;

int memoria_connection;
int file_system_connection;
int cpu_connection;


int main(void) {

	//Iniciamos tanto el log como el config
	logger = iniciar_logger();
	config = iniciar_config();

	//Inicializamos las variables globales desde el config, que loggee errores o success si todo esta bien
	log_info(logger,initial_setup());

	// Nos conectamos a los "servidores" (memoria, file system y CPU) como "clientes"
	if((memoria_connection = crear_conexion(memoria_ip,memoria_port)) != 0) log_info(logger, "Conexion establecida con la Memoria");
	if((file_system_connection = crear_conexion(file_system_ip,file_system_port)) != 0) log_info(logger, "Conexion establecida con el File System");
	if((cpu_connection = crear_conexion(cpu_ip,cpu_port)) != 0) log_info(logger, "Conexion establecida con la CPU");

	//NO HAGO HANDSHAKE con ninguno de los 3 (no lo pide la consigna)


	int server_connection = iniciar_servidor(server_port);

	log_info(logger, "Kernel listo para recibir Consolas");
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


	terminar_programa(server_connection, logger, config);

	return EXIT_SUCCESS;
}
void iterator(char* value) {
	log_info(logger,"%s", value);
}

void terminar_programa(int conexion, t_log* logger, t_config* config)
{
	/* Y por ultimo, hay que liberar lo que utilizamos (conexion, log y config)
	  con las funciones de las commons y del TP mencionadas en el enunciado */

	log_destroy(logger);
	config_destroy(config);
	liberar_conexion(conexion);
}
