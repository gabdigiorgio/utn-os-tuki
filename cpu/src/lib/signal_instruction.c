#include "../../includes/code_reader.h"

int ejecutar_signal(t_contexto* contexto, char* param1){
	contexto->param_length = strlen(param1) + 1;
	contexto->param = realloc(contexto->param,contexto->param_length);
	memcpy(contexto->param, param1, contexto->param_length);

	contexto_estado = SIGNAL;

	log_info(logger, "Ejecutando SIGNAL - [%s]", param1);

	return 1;
}
