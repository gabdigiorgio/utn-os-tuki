#include "../../includes/code_reader.h"

int ejecutar_mov_in(t_contexto* contexto){

	t_instruc_mem* instruccion = inicializar_instruc_mem();
	copiar_instruccion_mem(instruccion, contexto);
	instruccion->estado = MOV_IN;
	instruccion->param1 = contexto->param1;
	instruccion->param2 = contexto->param2; //traducir_direccion(contexto->param2) param2 es direccion logica
	instruccion->pid = contexto->pid;

	serializar_instruccion_memoria(memoria_connection, instruccion);

	//char* registro = esperar_valor_registro(memoria_connection);

	//log_info(logger, registro);

	//cambiar_registro(esperar_valor_registro(memoria_connection), seleccionar_registro(contexto->param1));

	return 0;
}

/*void cambiar_registro(char* registro, char* valor){
	copiar_string(valor,registro);
}*/
