#include "../../includes/code_reader.h"

void ejecutar_set(char* param1, char* param2){
	log_info(logger, "Ejecute SET");

	char** registro;

	cambiar_registro(seleccionar_registro(param1),param2);
}

void cambiar_registro(char* registro, char* valor){
	log_info(logger,valor);
	log_info(logger,registro);
	copiar_string(valor,registro);
}
