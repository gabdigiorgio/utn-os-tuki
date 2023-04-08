#include "file-system.h"

int conexion_cliente;
int conexion_servidor;
char* ip;
char* puerto_cliente;
char* puerto_server;
t_log* logger;
t_config* config;

int main(void)
{
	logger = iniciar_logger();
	config = iniciar_config();

	log_info(logger,initial_setup());

	// Creamos una conexión hacia el servidor (memoria)

	conexion_cliente = crear_conexion(ip, puerto_cliente);

	if(conexion_cliente != 0) log_info(logger, "Conexion con la memoria establecida correctamente");

	log_info(logger, handshake(conexion_cliente));

	conexion_servidor = iniciar_servidor(puerto_server);

	log_info(logger, "File system listo para recibir al Kernel");
	int cliente_fd = esperar_cliente(conexion_servidor);

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
}

char* initial_setup(){
	char* result = "";
	int error = 1;

	if(error == 1 && strcmp(ip = config_get_string_value(config,"IP_MEMORIA"),"") == 0){
		result = "No se pudo obtener la IP desde el archivo config";
		error = 0;
	}
	if(error == 1 && strcmp(puerto_server = config_get_string_value(config,"PUERTO_ESCUCHA"),"") == 0){
		result = "No se pudo obtener el puerto de escucha desde el archivo config";
		error = 0;
	}
	if(error == 1 && strcmp(puerto_cliente = config_get_string_value(config,"PUERTO_MEMORIA"),"") == 0){
		result = "No se pudo obtener el puerto de conexion desde el archivo config";
		error = 0;
	}
	if(error == 1) result = "Valores de configuracion leidos correctamente";

	return result;
}
