/*
 ============================================================================
 Name        : consola.c
 Author      : 
 Version     :
 Copyright   : Your copyright notice
 Description : Hello World in C, Ansi-style
 ============================================================================
 */

#include "../includes/consola.h"


int main(int argc, char *argv[]) {


	logger= iniciar_logger();
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

	//CREAMOS CONEXION HACIA EL SERVIDOR DE KERNEL

	kernel_connection=crear_conexion(kernel_ip,kernel_port);
	//enviar_mensaje(valor,conexion);
	//leo la consola
	leer_consola(logger);
	//armo paquete
	paquete(kernel_connection);
	terminar_programa(kernel_connection,logger,config);

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

void terminar_programa()
{
	log_destroy(logger);
	config_destroy(config);
	liberar_conexion(kernel_connection);

}
