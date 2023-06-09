#include "../includes/utils.h"

t_config* iniciar_config(char * path_config)
{
	t_config* nuevo_config;
	if((nuevo_config = config_create(path_config))==NULL){
		printf("No pude leer la config \n");
		exit(2);
	}
	return nuevo_config;
}

t_log* iniciar_logger(void)
{
	t_log* nuevo_logger = log_create("cpu.log", "CPU", 1, LOG_LEVEL_DEBUG);

	return nuevo_logger;
}

bool sort_list(t_instruc* instruc1, t_instruc* instruc2){
	return instruc1->nro < instruc2->nro;
}

void copiar_string(char* origen, char* destino){
	int largo_origen = strlen(origen);
	int largo_dest = strlen(destino);

	for(int i = 0; i<largo_origen; i++){
		destino[i] = origen[i];
	}
}

t_contexto* contexto_create(){
	t_contexto *contexto = malloc(sizeof(t_contexto));
	contexto->instrucciones = list_create();
	contexto->registros = inicializar_registros();
	contexto->param1 = malloc(sizeof(char) * 2);
	memcpy(contexto->param1, "0", (sizeof(char) * 2));
	contexto->param1_length = sizeof(char) * 2;
	contexto->param2 = malloc(sizeof(char) * 2);
	memcpy(contexto->param2, "0", (sizeof(char) * 2));
	contexto->param2_length = sizeof(char) * 2;
	contexto->param3 = malloc(sizeof(char) * 2);
	memcpy(contexto->param3, "0", (sizeof(char) * 2));
	contexto->param3_length = sizeof(char) * 2;
	contexto->estado = EXIT;
	contexto->pid = 0;

	return contexto;
}

void contexto_destroy(t_contexto* contexto){
	list_destroy_and_destroy_elements(contexto->instrucciones, (void*)instrucciones_destroy);
	free(contexto->registros);
	free(contexto->param1);
	free(contexto->param2);
	free(contexto->param3);
	free(contexto);
}

void instrucciones_destroy(t_instruc* instruccion){
	free(instruccion->instruct);
	free(instruccion->param1);
	free(instruccion->param2);
	free(instruccion->param3);
	free(instruccion);
}

t_registros* inicializar_registros()
{
	t_registros *registros = malloc(sizeof(t_registros));

	registros->ip = 0;
	memcpy(registros->ax, "0000", (sizeof(char) * 5));
	memcpy(registros->bx, "0000", (sizeof(char) * 5));
	memcpy(registros->cx, "0000", (sizeof(char) * 5));
	memcpy(registros->dx, "0000", (sizeof(char) * 5));
	memcpy(registros->eax, "00000000", (sizeof(char) * 9));
	memcpy(registros->ebx, "00000000", (sizeof(char) * 9));
	memcpy(registros->ecx, "00000000", (sizeof(char) * 9));
	memcpy(registros->edx, "00000000", (sizeof(char) * 9));
	memcpy(registros->rax, "0000000000000000", (sizeof(char) * 17));
	memcpy(registros->rbx, "0000000000000000", (sizeof(char) * 17));
	memcpy(registros->rcx, "0000000000000000", (sizeof(char) * 17));
	memcpy(registros->rdx, "0000000000000000", (sizeof(char) * 17));

	return registros;
}

t_instruc_mem* inicializar_instruc_mem()
{
	t_instruc_mem* instruccion = malloc(sizeof(t_instruc_mem));
	instruccion->pid=0;
	instruccion->param1 = malloc(sizeof(char) * 2);
	memcpy(instruccion->param1, "0", (sizeof(char) * 2));
	instruccion->param1_length = sizeof(char) * 2;
	instruccion->param2 = malloc(sizeof(char) * 2);
	memcpy(instruccion->param2, "0", (sizeof(char) * 2));
	instruccion->param2_length = sizeof(char) * 2;
	instruccion->param3 = malloc(sizeof(char) * 2);
	memcpy(instruccion->param3, "0", (sizeof(char) * 2));
	instruccion->param3_length = sizeof(char) * 2;
	instruccion->estado = CREATE_SEGMENT;

	return instruccion;
}

void copiar_instruccion_mem(t_instruc_mem* instruccion, t_contexto* contexto){

	instruccion->param1_length = contexto->param1_length;
	instruccion->param2_length = contexto->param2_length;
	instruccion->param3_length = contexto->param3_length;
	instruccion->estado = contexto->estado;
	instruccion->pid = contexto->pid;

	instruccion->param1 = realloc(instruccion->param1,instruccion->param1_length);
	instruccion->param2 = realloc(instruccion->param2,instruccion->param2_length);
	instruccion->param3 = realloc(instruccion->param3,instruccion->param3_length);

	memcpy(instruccion->param1,contexto->param1,instruccion->param1_length);
	memcpy(instruccion->param2,contexto->param2,instruccion->param2_length);
	memcpy(instruccion->param3,contexto->param3,instruccion->param3_length);

}
