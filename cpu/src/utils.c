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

	int asd = strcmp(instruccion->instruct,"SET");

	if(asd == 0)log_info(logger,"Ejecute SET");

	/* switch(instruccion->instruct){
		case "SET":
			log_info(logger,"Ejecute SET \b");
			break;
		case "MOV_IN":
			log_info(logger,"Ejecute MOV_IN \b");
			break;
		case "MOV_OUT":
			log_info(logger,"Ejecute MOV_OUT \b");
			break;
		case "I/O":
			log_info(logger,"Ejecute I/O \b");
			break;
		case "F_OPEN":
			log_info(logger,"Ejecute F_OPEN \b");
			break;
		case "F_CLOSE":
			log_info(logger,"Ejecute F_CLOSE \b");
			break;
		case "F_SEEK":
			log_info(logger,"Ejecute F_SEEK \b");
			break;
		case "F_READ":
			log_info(logger,"Ejecute F_READ \b");
			break;
		case "F_WRITE":
			log_info(logger,"Ejecute F_WRITE \b");
			break;
		case "F_TRUNCATE":
			log_info(logger,"Ejecute F_TRUNCATE \b");
			break;
		case "WAIT":
			log_info(logger,"Ejecute WAIT \b");
			break;
		case "SIGNAL":
			log_info(logger,"Ejecute SIGNAL \b");
			break;
		case "CREATE_SEGMENT":
			log_info(logger,"Ejecute CREATE_SEGMENT \b");
			break;
		case "DELETE_SEGMENT":
			log_info(logger,"Ejecute DELETE_SEGMENT \b");
			break;
		case "YIELD":
			log_info(logger,"Ejecute YIELD \b");
			break;
		case "EXIT":
			log_info(logger,"Ejecute EXIT \b");
			break;
		default:
			log_info(logger,"Instruccion desconocida \b");
			break;
	} */
}
