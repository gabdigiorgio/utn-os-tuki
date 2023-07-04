#include "../../../includes/code_reader.h"
#include "../../../includes/mmu.h"

int ejecutar_f_write_read(t_contexto *contexto, t_instruc *instruccion, contexto_estado_t estado)
{
	int dir_fisica = traducir_direccion(instruccion->param2, contexto);

	if(dir_fisica < 0) return 1;

	char* s_dir_fisica = string_itoa(dir_fisica);

	int size_dir = strlen(s_dir_fisica) + 1;

	char* params = string_new();
	memcpy(params,"",sizeof(char));

	contexto->param1_length = instruccion->param1_length;
	contexto->param1 = realloc(contexto->param1,contexto->param1_length);
	memcpy(contexto->param1,instruccion->param1,contexto->param1_length);

	contexto->param2_length = size_dir;
	contexto->param2 = realloc(contexto->param2,size_dir);
	memcpy(contexto->param2,s_dir_fisica,size_dir);

	contexto->param3_length = instruccion->param3_length;
	contexto->param3 = realloc(contexto->param3,contexto->param3_length);
	memcpy(contexto->param3,instruccion->param3,contexto->param3_length);

	string_append(&params,contexto->param1);
	string_append(&params," , ");
	string_append(&params,contexto->param2);
	string_append(&params," , ");
	string_append(&params,contexto->param3);

	contexto_estado = estado;

	log_instruccion(contexto->pid,instruccion->instruct,params);

	free(params);

	return 1;
}
