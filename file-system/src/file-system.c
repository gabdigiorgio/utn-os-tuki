#include "../includes/file-system.h"

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

	log_info(logger, handshake_client(memoria_connection,3)); //handshake(memoria_connection,1)

	int server_connection = iniciar_servidor(server_port);

	log_info(logger, "File System listo para recibir al Kernel");
	int connection_fd = esperar_cliente(server_connection);
	log_info(logger,handshake(connection_fd));
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

/*int main(int argc, char *argv[])
{
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

	// Creamos una conexión hacia el servidor (memoria)

	memoria_connection = crear_conexion(memoria_ip, memoria_port);

	if(memoria_connection != 0) log_info(logger, "Conexion con la memoria establecida correctamente");

	log_info(logger, handshake_cliente(memoria_connection,3));

	//memoria_connection = iniciar_servidor(server_port);
	int server_connection = iniciar_servidor(server_port);

	log_info(logger, "File system listo para recibir al Kernel");
	int cliente_fd = esperar_cliente(server_connection);
	log_info(logger,handshake(cliente_fd));

	t_list* lista;
	while (1) {
		int cod_op = recibir_operacion(cliente_fd);
		switch (cod_op) {
		case MENSAJE:
			recibir_mensaje(cliente_fd);
			break;
		case PAQUETE:
			lista = recibir_paquete(cliente_fd);
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
}*/
