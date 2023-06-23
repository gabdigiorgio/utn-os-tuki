#include "../../../includes/code_reader.h"

int ejecutar_mov_out(t_contexto *contexto, t_instruc *instruccion)
{
	int exit_status = 0;

	contexto->estado = MOV_OUT;
	contexto->param1_length = instruccion->param1_length;
	contexto->param1 = realloc(contexto->param1, contexto->param1_length);
	memcpy(contexto->param1, instruccion->param1, contexto->param1_length);

	contexto->param2_length = instruccion->param2_length;
	contexto->param2 = realloc(contexto->param2, contexto->param2_length);
	memcpy(contexto->param2, instruccion->param2, contexto->param2_length);

	//Hallo el valor del registro
	char *registro = seleccionar_registro(contexto->param2);
	contexto->param2_length = strlen(registro) + 1;
	contexto->param2 = realloc(contexto->param2, contexto->param2_length);
	memcpy(contexto->param2, registro, contexto->param2_length);

	log_info(logger, "Ejecutando [MOV_OUT]");

	exit_status = traducir_direccion(contexto->param1, contexto);

	if(exit_status != 0) return exit_status;

	t_instruc_mem *instruccion_mov_out = inicializar_instruc_mem();
	copiar_instruccion_mem(instruccion_mov_out, contexto);

	serializar_instruccion_memoria(memoria_connection, instruccion_mov_out);

	log_info(logger, "Accion: [MOV_OUT] - Valor: %s - Registro: %s", contexto->param2, instruccion->param2);

	return exit_status;
}
