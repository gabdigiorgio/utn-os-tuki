#include "../includes/comm_Mem.h"

void realizar_f_write(t_instruc_file* instruccion_file){
	t_instruc_mem* instruccion_mem = inicializar_instruc_mem();
	copiar_instruccion_mem(instruccion_mem,instruccion_file);
	serializar_instruccion_memoria(memoria_connection, instruccion_mem);

	char *valor = esperar_valor(memoria_connection);

	log_info(logger,"Valor %s", valor);
}

void realizar_f_read(t_instruc_file* instruccion_file){

	int id_fcb = buscar_fcb(instruccion_file->param1);
	//t_list* lista_de_bloques = armar_lista_offsets(id_fcb,instruccion_file->param3,instruccion_file->param4);
	//void* datos = leer_datos(lista_de_bloques);

	//t_instruc_mov* instruccion_mov = ionicializar_instruc_mov();
	//instruccion_mov->param1 = dir fisica
	//instruccion_mov->param2 = tamanio
	//instruccion_mov->param3 = datos

	//t_instruc_mem* instruccion_mem = inicializar_instruc_mem();
	//copiar_instruccion_mem(instruccion_mem,instruccion_file);
	//serializar_instruccion_memoria(memoria_connection, instruccion_mem);
}
