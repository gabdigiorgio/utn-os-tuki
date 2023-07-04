#include "../../../includes/code_reader.h"
#include "../../../includes/mmu.h"

int ejecutar_mov_in(t_contexto *contexto, t_instruc *instruccion)
{
	t_instruc_mov *instruccion_movimiento = inicializar_instruc_mov();
	char *registro = seleccionar_registro(instruccion->param2);
	char* placeholder = "0";
	uint32_t tamanio = strlen(registro);

	log_info(logger, "Ejecutando [MOV_IN]");
	int dir_fisica = traducir_direccion(instruccion->param1, contexto);

	if(dir_fisica < 0) return 1;

	generar_instruccion_mov(instruccion_movimiento,MOV_IN,dir_fisica,tamanio,placeholder);

	serializar_instruccion_mov(memoria_connection, instruccion_movimiento);

	void* valor = esperar_valor(memoria_connection);

	asignar_valor_registro(seleccionar_registro(instruccion->param2), valor, tamanio);

	free(valor);

	destroy_instruc_mov(instruccion_movimiento);

	log_info(logger, "Accion: [MOV_IN] - Valor: %s - Registro: %s", seleccionar_registro(contexto->param1), contexto->param1);

	return 0;
}