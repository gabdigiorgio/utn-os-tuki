#include "../includes/comm_Mem.h"

void realizar_f_write(t_instruc_file* instruccion_file){

	char* direccion_fisica = instruccion_file->param2;
	char* tamanio = instruccion_file->param3;
	char* placeholder = "0";


	t_instruc_mov *instruccion_mem = inicializar_instruc_mov();
	generar_instruccion_mov(instruccion_mem, F_WRITE, direccion_fisica, tamanio, placeholder);
	serializar_instruccion_mov(memoria_connection, instruccion_mem);

	void* datos = esperar_valor(memoria_connection);

	log_info(logger,"Valor %s", datos);

	int id_fcb = buscar_fcb(instruccion_file->param1);

	t_list* lista_de_bloques = armar_lista_offsets(id_fcb,tamanio,instruccion_file->param4);

	escribir_datos(datos, lista_de_bloques);

	destroy_instruc_mov(instruccion_mem);
}

void realizar_f_read(t_instruc_file* instruccion_file){

	char* direccion_fisica = instruccion_file->param2;
	char* tamanio = instruccion_file->param3;

	int id_fcb = buscar_fcb(instruccion_file->param1);

	t_list* lista_de_bloques = armar_lista_offsets(id_fcb,tamanio,instruccion_file->param4);
	void* datos = leer_datos(lista_de_bloques);

	t_instruc_mov *instruccion_mem = inicializar_instruc_mov();

	generar_instruccion_mov(instruccion_mem, F_READ, direccion_fisica, tamanio, datos);

	serializar_instruccion_mov(memoria_connection, instruccion_mem);

	destroy_instruc_mov(instruccion_mem);
}
