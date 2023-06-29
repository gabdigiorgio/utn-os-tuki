#include "../../../includes/code_reader.h"

int ejecutar_mov_out(t_contexto *contexto, t_instruc *instruccion)
{
	int exit_status = 0;

	t_instruc_mov *instruccion_movimiento = inicializar_instruc_mov();
	char *registro = seleccionar_registro(instruccion->param2);
	uint32_t tamanio = strlen(registro);

	log_info(logger, "Ejecutando [MOV_OUT]");

	int dir_fisica = traducir_direccion(instruccion->param1, contexto);

	if(dir_fisica < 0) return 1;

	generar_instruccion_mov(instruccion_movimiento,MOV_OUT,dir_fisica,tamanio,registro);

	serializar_instruccion_mov(memoria_connection, instruccion_movimiento);

	destroy_instruc_mov(instruccion_movimiento);

	log_info(logger, "Accion: [MOV_OUT] - Valor: %s - Registro: %s", contexto->param2, instruccion->param2);

	return exit_status;
}
