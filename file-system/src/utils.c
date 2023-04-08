#include "utils.h"

t_config* iniciar_config(void)
{
	t_config* nuevo_config;
	if((nuevo_config = config_create("./file-system.config"))==NULL){
		printf("No pude leer la config \n");
		exit(2);
	}
	return nuevo_config;
}

t_log* iniciar_logger(void)
{
	t_log* nuevo_logger = log_create("./file-system.log", "File system", true, LOG_LEVEL_INFO);

	return nuevo_logger;
}
