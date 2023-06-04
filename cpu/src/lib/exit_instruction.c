#include "../../includes/code_reader.h"

int ejecutar_exit(t_contexto* contexto){
	char* placeholder = "0";
	contexto->param1_length = strlen(placeholder) + 1;
	contexto->param1 = realloc(contexto->param1,contexto->param1_length);
	memcpy(contexto->param1, placeholder, contexto->param1_length);

	log_instruccion("EXIT","");
	contexto_estado = EXIT;

	return 1;
}
