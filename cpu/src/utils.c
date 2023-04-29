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
	t_log* nuevo_logger = log_create("cpu.log", "CPU", 1, LOG_LEVEL_DEBUG);

	return nuevo_logger;
}

void ejecutar_instruccion(t_instruc* instruccion){

	leer_instruccion(instruccion);
}

bool sort_list(t_instruc* instruc1, t_instruc* instruc2){
	return instruc1->nro < instruc2->nro;
}
