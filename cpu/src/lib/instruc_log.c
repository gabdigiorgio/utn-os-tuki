#include "../../includes/code_reader.h"

void log_instruccion (char* instruc, char* params){
	log_info(logger, "Ejecutando [%s] - [%s]", instruc, params);
}
