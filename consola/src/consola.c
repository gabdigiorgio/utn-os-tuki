/*
 ============================================================================
 Name        : consola.c
 Author      : 
 Version     :
 Copyright   : Your copyright notice
 Description : Hello World in C, Ansi-style
 ============================================================================
 */

#include "consola.h"
#include<readline/readline.h>

int main(void) {

	int conexion;
		char* ip;
		char* puerto;
		char* valor;

		t_log* logger;
		t_config* config;

		logger= iniciar_logger();
		config=iniciar_config();

		ip=config_get_string_value(config,"IP_KERNEL");
		puerto=config_get_string_value(config,"PUERTO_KERNEL");

		//CREAMOS CONEXION HACIA EL SERVIDOR DE KERNEL

		conexion=crear_conexion(ip,puerto);
		//enviar_mensaje(valor,conexion);
		//leo la consola
		leer_consola(logger);
		//armo paquete
		paquete(conexion);
		terminar_programa(conexion,logger,config);

}

 t_log* iniciar_logger(void)
{
  t_log* nuevo_logger;
  nuevo_logger=log_create("./consola.log","consola",1,LOG_LEVEL_INFO);

  return nuevo_logger;
}

 t_config* iniciar_config(void)
{
	t_config* nuevo_config=config_create("./consola.config");

	return nuevo_config;
}

void leer_consola(t_log* logger)
{
	char* leido;

	while(1){
			leido = readline(" > ");
			if(strcmp(leido,"") == 0){break;}
			log_info(logger,leido);
			free(leido);
		}
		free(leido);



}

void paquete(int conexion)
{
	char* leido;
	t_paquete* paquete=crear_paquete();

	while(1){
				leido = readline("ingrese un valor del paquete > ");
				if(strcmp(leido, "") == 0){break;}
				agregar_a_paquete(paquete,leido,strlen(leido)+1);
				free(leido);
			}
	    enviar_paquete(paquete,conexion);
	    puts("Mensaje enviada al servidor kernel, con exito!");
	    eliminar_paquete(paquete);
}

void terminar_programa(int conexion, t_log* logger, t_config* config)
{
	log_destroy(logger);
	config_destroy(config);
	liberar_conexion(conexion);

}
