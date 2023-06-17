#include "../../includes/code_reader.h"
#include "../../includes/mmu.h"

int ejecutar_mov_in(t_contexto *contexto, t_instruc *instruccion)
{
	contexto->estado = MOV_IN;
	contexto->param1_length = instruccion->param1_length;
	contexto->param1 = realloc(contexto->param1, contexto->param1_length);
	memcpy(contexto->param1, instruccion->param1, contexto->param1_length);

	contexto->param2_length = instruccion->param2_length;
	contexto->param2 = realloc(contexto->param2, contexto->param2_length);
	memcpy(contexto->param2, instruccion->param2, contexto->param2_length);

	log_info(logger, "Ejecutando [MOV_IN] - [%s , %s]", instruccion->param1, instruccion->param2);

	traducir_direccion(contexto->param2, contexto);

	t_instruc_mem *instruccion_memoria = inicializar_instruc_mem();
	copiar_instruccion_mem(instruccion_memoria, contexto);

	serializar_instruccion_memoria(memoria_connection, instruccion_memoria);

	//char *registro = esperar_valor_registro(memoria_connection);

	//log_info(logger, registro);

	//cambiar_registro(registro, seleccionar_registro(contexto->param1));

	return 0;
}

/*void cambiar_registro(char *registro, char *valor)
 {
 copiar_string(valor, registro);
 }*/

/*char* esperar_valor_registro(int memoria_connection)
 {
 t_paquete *paquete = malloc(sizeof(t_paquete));
 paquete->buffer = malloc(sizeof(t_buffer));
 deserializar_header(paquete, server_connection);

 switch (paquete->codigo_operacion)
 {
 case 1:

 }
 }*/
