#include "../../includes/code_reader.h"

int ejecutar_io(t_contexto* contexto, char* param1){
	contexto->param_length = strlen(param1) + 1;
	contexto->param = realloc(contexto->param,contexto->param_length);
	memcpy(contexto->param, param1, contexto->param_length);

	contexto_estado = IO;

	log_info(logger, "Ejecute I/O");

	return 1;
}
