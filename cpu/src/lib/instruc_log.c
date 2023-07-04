#include "../../includes/code_reader.h"

void log_instruccion (uint32_t pid, char* instruc, char* params){
	log_info(logger, "PID: %d - Ejecutando [%s] - [%s]",pid, instruc, params);
}
