#include "../../../includes/code_reader.h"

int ejecutar_syscall(t_contexto* contexto, t_instruc* instruccion, contexto_estado_t estado,int cant_params){
	char* params = string_new();
	memcpy(params,"",sizeof(char));
	int offset = 0;

	switch(cant_params){
		case 1:
			contexto->param1_length = instruccion->param1_length;
			contexto->param1 = realloc(contexto->param1,contexto->param1_length);
			memcpy(contexto->param1,instruccion->param1,contexto->param1_length);

			string_append(&params,contexto->param1);
			break;
		case 2:
			contexto->param1_length = instruccion->param1_length;
			contexto->param1 = realloc(contexto->param1,contexto->param1_length);
			memcpy(contexto->param1,instruccion->param1,contexto->param1_length);

			contexto->param2_length = instruccion->param2_length;
			contexto->param2 = realloc(contexto->param2,contexto->param2_length);
			memcpy(contexto->param2,instruccion->param2,contexto->param2_length);

			string_append(&params,contexto->param1);
			string_append(&params," , ");
			string_append(&params,contexto->param2);

			break;
		case 3:
			contexto->param1_length = instruccion->param1_length;
			contexto->param1 = realloc(contexto->param1,contexto->param1_length);
			memcpy(contexto->param1,instruccion->param1,contexto->param1_length);

			contexto->param2_length = instruccion->param2_length;
			contexto->param2 = realloc(contexto->param2,contexto->param2_length);
			memcpy(contexto->param2,instruccion->param2,contexto->param2_length);

			contexto->param3_length = instruccion->param3_length;
			contexto->param3 = realloc(contexto->param3,contexto->param3_length);
			memcpy(contexto->param3,instruccion->param3,contexto->param3_length);

			string_append(&params,contexto->param1);
			string_append(&params," , ");
			string_append(&params,contexto->param2);
			string_append(&params," , ");
			string_append(&params,contexto->param3);
			break;
		default:
			break;
	}

	contexto_estado = estado;

	log_instruccion(contexto->pid,instruccion->instruct,params);

	free(params);

	return 1;
}
