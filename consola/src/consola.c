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

	instruc_list = list_create();

	logger= iniciar_logger();
	  if (argc < 2) {
		log_error(logger, "Falta parametro del path del archivo de configuracion");
		return EXIT_FAILURE;
	  } else if (argc < 3){
		log_error(logger, "Falta parametro del path del archivo de pseudo-codigo");
		return EXIT_FAILURE;
	  }
	  config = iniciar_config(argv[1]);

	//Inicializamos las variables globales desde el config, que loggee errores o success si todo esta bien
	int exit_status = initial_setup();
	if (exit_status==EXIT_FAILURE){
		return EXIT_FAILURE;
	}

	//Leemos el archivo de pseudo-codigo
	exit_status = leer_pseudocodigo(argv[2]);
	if (exit_status==EXIT_FAILURE){
		return EXIT_FAILURE;
	}
	//CREAMOS CONEXION HACIA EL SERVIDOR DE KERNEL

	kernel_connection=crear_conexion(kernel_ip,kernel_port);

	serializar_instrucciones(kernel_connection,instruc_list);

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

void terminar_programa()
{
	log_destroy(logger);
	config_destroy(config);
	liberar_conexion(kernel_connection);

}

int leer_pseudocodigo(char* path_pseudocodigo){
	FILE *fp = fopen(path_pseudocodigo, "r");
	int nro = 0;

	if (fp == NULL){
		log_error(logger, "No se pudo leer el archivo de pseudocodigo");
		return EXIT_FAILURE;
	}

	const unsigned MAX_LENGTH = 256;
	char buffer[MAX_LENGTH];

	while (fgets(buffer, MAX_LENGTH, fp)){
		char** parameters = string_split(buffer," ");
		t_instruc *instruccion = malloc(sizeof *instruccion);

		instruccion->nro = nro;
		instruccion->instruct = string_replace(parameters[0],"\n","");
		instruccion->instruct_length = strlen(instruccion->instruct) + 1;
		instruccion->param1 = "";
		instruccion->param1_length = strlen(instruccion->param1) + 1;
		instruccion->param2 = "";
		instruccion->param2_length = strlen(instruccion->param2) + 1;
		instruccion->param3 = "";
		instruccion->param3_length = strlen(instruccion->param3) + 1;

		if (parameters[1] != NULL) {
			instruccion->param1 = string_replace(parameters[1],"\n","");
			instruccion->param1_length = strlen(instruccion->param1) + 1;
		}

		if (parameters[1] != NULL && parameters[2] != NULL) {
			instruccion->param2 = string_replace(parameters[2],"\n","");
			instruccion->param2_length = strlen(instruccion->param2) + 1;
		}

		if (parameters[1] != NULL && parameters[2] != NULL && parameters[3] != NULL) {
			instruccion->param3 = string_replace(parameters[3],"\n","");
			instruccion->param3_length = strlen(instruccion->param3) + 1;
		}

		nro++;

		list_add(instruc_list,instruccion);
	}

	fclose(fp);

	return EXIT_SUCCESS;
}
