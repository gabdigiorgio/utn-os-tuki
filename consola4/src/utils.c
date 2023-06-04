#include "../includes/utils.h"

t_config* iniciar_config(char * path_config)
{
	t_config* nuevo_config;
	if((nuevo_config = config_create(path_config))==NULL){
		printf("No pude leer la config \n");
		exit(2);
	}
	return nuevo_config;
}

t_log* iniciar_logger(void)
{
	t_log* nuevo_logger = log_create("consola.log", "CONSOLA", 1, LOG_LEVEL_DEBUG);

	return nuevo_logger;
}

t_instruc* crear_instruccion(){
	t_instruc* instruccion = malloc(sizeof(t_instruc));
	instruccion->instruct = malloc(sizeof(char));
	instruccion->instruct_length = sizeof(char);
	memcpy(instruccion->instruct, "", (sizeof(char)));
	instruccion->param1 = malloc(sizeof(char));
	memcpy(instruccion->param1, "", (sizeof(char)));
	instruccion->param1_length = sizeof(char);
	instruccion->param2 = malloc(sizeof(char));
	memcpy(instruccion->param2, "", (sizeof(char)));
	instruccion->param2_length = sizeof(char);
	instruccion->param3 = malloc(sizeof(char));
	memcpy(instruccion->param3, "", (sizeof(char)));
	instruccion->param3_length = sizeof(char);

	return instruccion;
}

void instrucciones_destroy(t_instruc* instruccion){
	free(instruccion->instruct);
	free(instruccion->param1);
	free(instruccion->param2);
	free(instruccion->param3);
	free(instruccion);
}
