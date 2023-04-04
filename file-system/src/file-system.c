#include "file-system.h"

int main(void)
{
	int conexion;
	char* ip;
	char* puerto;

	t_log* logger;
	t_config* config;

	logger = iniciar_logger();
	config = iniciar_config();

	ip = config_get_string_value(config,"IP_MEMORIA");
	puerto = config_get_string_value(config,"PUERTO_MEMORIA");

	log_info(logger, "IP de la memoria: %s", ip);
	log_info(logger, "Puerto de la memoria: %s", puerto);

	// Creamos una conexión hacia el servidor (memoria)

	conexion = crear_conexion(ip, puerto);

	paquete(conexion);

	terminar_programa(conexion, logger, config);

}

t_log* iniciar_logger(void)
{
	t_log* nuevo_logger = log_create("./file-system.log","File system",true,LOG_LEVEL_INFO);

	return nuevo_logger;
}

t_config* iniciar_config(void)
{
	t_config* nuevo_config = config_create("./file-system.config");

	return nuevo_config;
}

void leer_consola(t_log* logger)
{
	char* leido;

	while(1){
		leido = readline("> ");

		if(strcmp(leido,"")==0)break;

		log_info(logger,leido);
		free(leido);
	}

}

void paquete(int conexion)
{

	char* leido;
	t_paquete* paquete = crear_paquete();

	while(1){
		leido = readline("> ");

		if(strcmp(leido,"")==0){
			break;
		}
		agregar_a_paquete(paquete,leido,(strlen(leido)+1));
		free(leido);
	}

	enviar_paquete(paquete,conexion);
	eliminar_paquete(paquete);

}

void terminar_programa(int conexion, t_log* logger, t_config* config)
{
	log_destroy(logger);
	config_destroy(config);
	liberar_conexion(conexion);
}
