#include "../../includes/code_reader.h"

int ejecutar_yield(t_contexto* contexto){
	char* placeholder = "0";
	contexto->param_length = strlen(placeholder) + 1;
	contexto->param = realloc(contexto->param,contexto->param_length);
	memcpy(contexto->param, placeholder, contexto->param_length);

	log_info(logger, "Ejecutando YIELD");
	contexto_estado = YIELD;

	return 1;
}
