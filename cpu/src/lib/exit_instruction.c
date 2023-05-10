#include "../../includes/code_reader.h"

int ejecutar_exit(){
	char* placeholder = "0";
	contexto->param_length = strlen(placeholder) + 1;
	contexto->param = malloc(contexto->param_length);
	memcpy(contexto->param, placeholder, contexto->param_length);

	log_info(logger, "Ejecute EXIT");
	contexto_estado = EXIT;

	return 1;
}
