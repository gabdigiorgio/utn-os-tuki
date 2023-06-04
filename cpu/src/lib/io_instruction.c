#include "../../includes/code_reader.h"

int ejecutar_io(t_contexto* contexto, char* param1){
	contexto->param1_length = strlen(param1) + 1;
	contexto->param1 = realloc(contexto->param1,contexto->param1_length);
	memcpy(contexto->param1, param1, contexto->param1_length);

	contexto_estado = IO;

	log_info(logger, "Ejecutando IO - [%s]", param1);

	return 1;
}
