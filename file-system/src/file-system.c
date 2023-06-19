#include "../includes/file-system.h"

int main(int argc, char *argv[]) {
	logger = iniciar_logger();

	//Inicializamos las variables globales desde el config, que loggee errores o success si todo esta bien
	if (argc < 2) {
		 log_error(logger, "Falta parametro del path del archivo de configuracion");
		 return EXIT_FAILURE;
	 }
	config = iniciar_config(argv[1]);
	int exit_status = initial_setup();
	if (exit_status==EXIT_FAILURE){
		return EXIT_FAILURE;
	}

	superbloque_config = iniciar_config(path_superbloque);
	exit_status = leer_superbloque_config();
	if (exit_status==EXIT_FAILURE){
		return EXIT_FAILURE;
	}

	//Inicializamos conexion con memoria
	if((memoria_connection = crear_conexion(memoria_ip,memoria_port)) == 0 || handshake_cliente(memoria_connection,3,4) == -1) {
		terminar_programa();
		return EXIT_FAILURE;
	}
	//log_info(logger,"%d",tamanio_archivo);
	int server_connection = iniciar_servidor(server_port);
	generar_fcb();
	log_info(logger, "File System listo para recibir al Kernel");
	//log_info(logger,"%d",tamanio_archivo);
	int connection_fd = esperar_cliente(server_connection);
	log_info(logger,handshake(connection_fd));
	t_list* lista;

	while(1){};

	return EXIT_SUCCESS;
}
void iterator(char* value) {
	log_info(logger,"%s", value);
}

void generar_fcb(){
	if (config_has_property(fcb, "NOMBRE_ARCHIVO")){
			nombre_archivo = config_get_string_value(fcb, "NOMBRE_ARCHIVO");
			log_info(logger, "%s", nombre_archivo);
		}
	if (config_has_property(fcb, "TAMANIO_ARCHIVO")){
			tamanio_archivo = config_get_int_value(fcb, "TAMANIO_ARCHIVO");
			log_info(logger, "%d", tamanio_archivo);
		}
	if (config_has_property(fcb, "PUNTERO_DIRECTO")){
			puntero_directo = config_get_int_value(fcb, "PUNTERO_DIRECTO");
			log_info(logger, "%d", puntero_directo);
		}
	if (config_has_property(fcb, "PUNTERO_INDIRECTO")){
			puntero_indirecto = config_get_int_value(fcb, "PUNTERO_INDIRECTO");
			log_info(logger, "%d", puntero_indirecto);
		}
	log_info(logger, "FCB Creado");
}

void terminar_programa()
{
	log_destroy(logger);
	config_destroy(config);
	config_destroy(superbloque_config);
	config_destroy(fcb);
	liberar_conexion(memoria_connection);
}
