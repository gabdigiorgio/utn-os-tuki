#include "../../includes/code_reader.h"

int ejecutar_wait(char* param1){
	contexto->param_length = strlen(param1) + 1;
	contexto->param = malloc(contexto->param_length);
	memcpy(contexto->param, param1, contexto->param_length);

	contexto_estado = IO;

	log_info(logger, "Ejecute Wait");

	return 1;
}
