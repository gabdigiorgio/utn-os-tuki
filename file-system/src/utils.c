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

fcb_t* iniciar_fcb(char* path_fcb){
	fcb_t* nuevo_fcb;
	if((nuevo_fcb = config_create(path_fcb))==NULL) {
		printf("No pude leer la config \n");
		exit(2);
	}
	return nuevo_fcb;
}

t_log* iniciar_logger(void)
{
	t_log* nuevo_logger = log_create("file-system.log", "FILE-SYSTEM", 1, LOG_LEVEL_DEBUG);

	return nuevo_logger;
}
