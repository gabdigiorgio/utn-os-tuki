#include "../../includes/code_reader.h"

int ejecutar_set(char* param1, char* param2){
	log_info(logger, "Ejecutamdp SET - [%s %s]", param1, param2);

	char** registro;

	cambiar_registro(seleccionar_registro(param1),param2);

	return 0;
}

void cambiar_registro(char* registro, char* valor){
	//log_info(logger,valor);
	//log_info(logger,registro);
	copiar_string(valor,registro);
}
