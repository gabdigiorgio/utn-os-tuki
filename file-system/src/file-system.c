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

	if((memoria_connection = crear_conexion(memoria_ip,memoria_port)) == 0 || handshake_cliente(memoria_connection,3,4) == -1) {
		terminar_programa();
		return EXIT_FAILURE;
	}

	int server_connection = iniciar_servidor(server_port);

	log_info(logger, "File System listo para recibir al Kernel");
	int connection_fd = esperar_cliente(server_connection);
	log_info(logger,handshake(connection_fd));
	t_list* lista;

	while(1){};

	return EXIT_SUCCESS;
}
void iterator(char* value) {
	log_info(logger,"%s", value);
}

void terminar_programa()
{
	log_destroy(logger);
	config_destroy(config);
	liberar_conexion(memoria_connection);
}
