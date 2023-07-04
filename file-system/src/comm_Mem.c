#include "../includes/comm_Mem.h"

void realizar_f_write(t_instruc_file* instruccion_file){
	t_instruc_mem* instruccion_mem = inicializar_instruc_mem();
	copiar_instruccion_mem(instruccion_mem,instruccion_file);
	serializar_instruccion_memoria(memoria_connection, instruccion_mem);

	char *valor = esperar_valor(memoria_connection);

	log_info(logger,"Valor %s", valor);

	// Se debe agregar el valor obtenido en el bloque correspondiente a partir del puntero
	// para saber los bloques debo a partir del punteo calcular el bloque inicial y con el tamaño
	// la cantidad minima de bloques que tengo leer para copiar y para el último bloque leer cuanto tengo que copiar (no todo sino lo bytes restantes)
}

void realizar_f_read(t_instruc_file* instruccion_file){
	t_instruc_mem* instruccion_mem = inicializar_instruc_mem();
	copiar_instruccion_mem(instruccion_mem,instruccion_file);
	serializar_instruccion_memoria(memoria_connection, instruccion_mem);
}
