#include "../../includes/code_reader.h"
#include "../../includes/mmu.h"

int ejecutar_mov_in(t_contexto *contexto, t_instruc *instruccion)
{
	int exit_status = 0;

	contexto->estado = MOV_IN;
	contexto->param1_length = instruccion->param1_length;
	contexto->param1 = realloc(contexto->param1, contexto->param1_length);
	memcpy(contexto->param1, instruccion->param1, contexto->param1_length);

	contexto->param2_length = instruccion->param2_length;
	contexto->param2 = realloc(contexto->param2, contexto->param2_length);
	memcpy(contexto->param2, instruccion->param2, contexto->param2_length);

	log_info(logger, "Ejecutando [MOV_IN] - [%s , %s]", instruccion->param1, instruccion->param2);

	exit_status = traducir_direccion(contexto->param2, contexto);

	if(exit_status != 0) return exit_status;

	t_instruc_mem *instruccion_memoria = inicializar_instruc_mem();
	copiar_instruccion_mem(instruccion_memoria, contexto);

	serializar_instruccion_memoria(memoria_connection, instruccion_memoria);

	char *valor = esperar_valor(memoria_connection);

	cambiar_registro(seleccionar_registro(contexto->param1), valor);

	return exit_status;
}

char* esperar_valor(int memoria_connection)
{
	t_paquete *paquete = malloc(sizeof(t_paquete));
	paquete->buffer = malloc(sizeof(t_buffer));
	deserializar_header(paquete, memoria_connection);

	switch (paquete->codigo_operacion)
	{
	case 1:
		t_instruc_mem *nueva_instruccion = inicializar_instruc_mem();
		deserializar_instruccion_memoria(nueva_instruccion, paquete->buffer, paquete->lineas);
		log_info(logger, "Me llego de memoria el valor: %s", nueva_instruccion->param2);
		return nueva_instruccion->param2;
		break;
	default:
		log_error(logger, "Fallo respuesta memoria a CPU");
		return NULL;
		break;
	}

	free(paquete->buffer->stream);
	free(paquete->buffer);
	free(paquete);
}
