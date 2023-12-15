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

	t_list* instruc_list = list_create();

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
	exit_status = leer_pseudocodigo(argv[2], instruc_list);
	if (exit_status==EXIT_FAILURE){

		return EXIT_FAILURE;
	}
	//CREAMOS CONEXION HACIA EL SERVIDOR DE KERNEL

	kernel_connection=crear_conexion(kernel_ip,kernel_port);

	serializar_instrucciones(kernel_connection,instruc_list);

	switch(recibir_respuesta(kernel_connection)){
		case 0:
			log_error(logger, "Error en la ejecucion");
			break;
		case 1:
			log_info(logger, "Ejecucion terminada");
			break;
		default:
			log_error(logger, "Hubo un error intesperado");
			break;
	}
	list_destroy_and_destroy_elements(instruc_list, (void*)instrucciones_destroy);

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

int leer_pseudocodigo(char* path_pseudocodigo, t_list* lista){
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
		t_instruc *instruccion = crear_instruccion();

		instruccion->nro = nro;

		parameters[0] = string_replace(parameters[0],"\n","");
		instruccion->instruct_length = strlen(parameters[0]) + 1;
		instruccion->instruct = realloc(instruccion->instruct,instruccion->instruct_length);
		memcpy(instruccion->instruct,parameters[0],instruccion->instruct_length);
		free(parameters[0]);


		if (parameters[1] != NULL) {
			parameters[1] = string_replace(parameters[1],"\n","");
			instruccion->param1_length = strlen(parameters[1]) + 1;
			instruccion->param1 = realloc(instruccion->param1,instruccion->param1_length);
			memcpy(instruccion->param1,parameters[1],instruccion->param1_length);

			if (parameters[2] != NULL) {
				parameters[2] = string_replace(parameters[2],"\n","");
				instruccion->param2_length = strlen(parameters[2]) + 1;
				instruccion->param2 = realloc(instruccion->param2,instruccion->param2_length);
				memcpy(instruccion->param2,parameters[2],instruccion->param2_length);

				if (parameters[3] != NULL) {
					parameters[3] = string_replace(parameters[3],"\n","");
					instruccion->param3_length = strlen(parameters[3]) + 1;
					instruccion->param3 = realloc(instruccion->param3,instruccion->param3_length);
					memcpy(instruccion->param3,parameters[3],instruccion->param3_length);
				}
				free(parameters[3]);
			}
			free(parameters[2]);
		}
		free(parameters[1]);

		nro++;

		free(parameters);

		list_add(lista,instruccion);
	}

	fclose(fp);

	return EXIT_SUCCESS;
}
