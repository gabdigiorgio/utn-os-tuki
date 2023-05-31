#include "../includes/utils.h"

t_config* iniciar_config(char *path_config) {
	t_config *nuevo_config;
	if ((nuevo_config = config_create(path_config)) == NULL) {
		printf("No pude leer la config \n");
		exit(2);
	}
	return nuevo_config;
}

t_log* iniciar_logger(void) {
	t_log *nuevo_logger = log_create("kernel.log", "KERNEL", 1,
			LOG_LEVEL_DEBUG);

	return nuevo_logger;
}

t_lista_mutex* init_list_mutex() {
	t_lista_mutex *list = malloc(sizeof(t_lista_mutex));

	list->lista = list_create();
	pthread_mutex_init(&(list->mutex), NULL);
	return list;
}

void list_push(t_lista_mutex *list, void *info) {
	pthread_mutex_lock(&(list->mutex));
	list_add(list->lista, info);
	pthread_mutex_unlock(&(list->mutex));
}

void* list_pop(t_lista_mutex *list) {
	pthread_mutex_lock(&(list->mutex));
	void *info = list_remove(list->lista, 0);
	pthread_mutex_unlock(&(list->mutex));
	return info;
}

void list_mutex_destroy(t_lista_mutex *list) {
	list_destroy(list->lista);
	pthread_mutex_destroy(&(list->mutex));
	free(list);
}

bool list_mutex_is_empty(t_lista_mutex *list) {
	pthread_mutex_lock(&(list->mutex));
	bool is_empty = list_is_empty(list->lista);
	pthread_mutex_unlock(&(list->mutex));
	return is_empty;
}

int list_mutex_size(t_lista_mutex *list) {
	pthread_mutex_lock(&(list->mutex));
	int size = list_size(list->lista);
	pthread_mutex_unlock(&(list->mutex));
	return size;
}

void* list_mutex_get(t_lista_mutex *list, int index){
	pthread_mutex_lock(&(list->mutex));
	void* element = list_get(list->lista, index);
	pthread_mutex_unlock(&(list->mutex));
	return element;
}

t_recurso* buscar_recurso(t_lista_mutex *lista_recursos, const char *nombre_recurso) {
	for (int i = 0; i < list_mutex_size(lista_recursos); i++) {
		t_recurso *recurso = (t_recurso*) list_mutex_get(lista_recursos, i);
		if (strcmp(recurso->nombre_recurso, nombre_recurso) == 0) {
			return recurso;
		}
	}
	return NULL;
}

bool recurso_existe_en_lista(t_lista_mutex *lista_recursos, const char *nombre_recurso) {
	t_recurso* recurso = buscar_recurso(lista_recursos, nombre_recurso);
	return (recurso != NULL);
}

void restar_instancia(t_lista_mutex *lista_recursos, const char *nombre_recurso) {
	t_recurso* recurso = buscar_recurso(lista_recursos, nombre_recurso);
	pthread_mutex_lock(&(recurso->mutex_instancias));
	recurso->instancias--;
	pthread_mutex_unlock(&(recurso->mutex_instancias));
}

void sumar_instancia(t_lista_mutex *lista_recursos, const char *nombre_recurso) {
	t_recurso* recurso = buscar_recurso(lista_recursos, nombre_recurso);
	pthread_mutex_lock(&(recurso->mutex_instancias));
	recurso->instancias++;
	pthread_mutex_unlock(&(recurso->mutex_instancias));
}

int obtener_instancias(t_lista_mutex *lista_recursos, const char* nombre_recurso) {
	t_recurso* recurso = buscar_recurso(lista_recursos, nombre_recurso);
	return recurso->instancias;
}

char* armar_lista_pids(t_list* lista){
	int largo = list_size(lista);
	char* pids = string_new();
	string_append(&pids, "Cola Ready %s : ");

	for(int i = 0; i < largo; i++){
		pcb_t* pcb = list_get(lista, i);
		char* pid = string_from_format("%u",pcb->pid);
		string_append(&pids," ");
		string_append(&pids,pid);

		free(pid);
	}

	return pids;
}

void instrucciones_destroy(t_instruc* instruccion){
	free(instruccion->instruct);
	free(instruccion->param1);
	free(instruccion->param2);
	free(instruccion->param3);
	free(instruccion);
}

void copiar_string(char* origen, char* destino){
	int largo_origen = strlen(origen);
	int largo_dest = strlen(destino);

	memcpy(destino,origen,largo_origen);
}

t_registros* inicializar_registros(){
	t_registros* registros = malloc(sizeof(t_registros));

	registros->ip = 0;
	memcpy(registros->ax,"0000",(sizeof(char) * 5));
	memcpy(registros->bx,"0000",(sizeof(char) * 5));
	memcpy(registros->cx,"0000",(sizeof(char) * 5));
	memcpy(registros->dx,"0000",(sizeof(char) * 5));
	memcpy(registros->eax,"00000000",(sizeof(char) * 9));
	memcpy(registros->ebx,"00000000",(sizeof(char) * 9));
	memcpy(registros->ecx,"00000000",(sizeof(char) * 9));
	memcpy(registros->edx,"00000000",(sizeof(char) * 9));
	memcpy(registros->rax,"0000000000000000",(sizeof(char) * 17));
	memcpy(registros->rbx,"0000000000000000",(sizeof(char) * 17));
	memcpy(registros->rcx,"0000000000000000",(sizeof(char) * 17));
	memcpy(registros->rdx,"0000000000000000",(sizeof(char) * 17));

	return registros;
}

void copiar_registros(t_registros* to_registros, t_registros* from_registros){
	to_registros->ip = from_registros->ip;
	copiar_string(from_registros->ax,to_registros->ax);
	copiar_string(from_registros->bx,to_registros->bx);
	copiar_string(from_registros->cx,to_registros->cx);
	copiar_string(from_registros->dx,to_registros->dx);
	copiar_string(from_registros->eax,to_registros->eax);
	copiar_string(from_registros->ebx,to_registros->ebx);
	copiar_string(from_registros->ecx,to_registros->ecx);
	copiar_string(from_registros->edx,to_registros->edx);
	copiar_string(from_registros->rax,to_registros->rax);
	copiar_string(from_registros->rbx,to_registros->rbx);
	copiar_string(from_registros->rcx,to_registros->rcx);
	copiar_string(from_registros->rdx,to_registros->rdx);
}

t_list* copiar_lista_instrucciones(t_list* nueva_lista, t_list* lista_instrucciones){
	int lineas = list_size(lista_instrucciones);

	for(int i = 0; i < lineas; i++){
			t_instruc* instrucciones = list_get(lista_instrucciones, i);
			t_instruc* nueva_instruccion = malloc(sizeof(t_instruc));

			memcpy(&nueva_instruccion->nro, &instrucciones->nro, sizeof(uint32_t));

			memcpy(&nueva_instruccion->instruct_length, &instrucciones->instruct_length, sizeof(uint32_t));

			nueva_instruccion->instruct = malloc(instrucciones->instruct_length);
			memcpy(nueva_instruccion->instruct, instrucciones->instruct, instrucciones->instruct_length);

			memcpy(&nueva_instruccion->param1_length, &instrucciones->param1_length, sizeof(uint32_t));

			nueva_instruccion->param1 = malloc(instrucciones->param1_length);
			memcpy(nueva_instruccion->param1, instrucciones->param1, instrucciones->param1_length);

			memcpy(&nueva_instruccion->param2_length, &instrucciones->param2_length, sizeof(uint32_t));

			nueva_instruccion->param2 = malloc(instrucciones->param2_length);
			memcpy(nueva_instruccion->param2, instrucciones->param2, instrucciones->param2_length);

			memcpy(&nueva_instruccion->param3_length, &instrucciones->param3_length, sizeof(uint32_t));

			nueva_instruccion->param3 = malloc(instrucciones->param3_length);
			memcpy(nueva_instruccion->param3, instrucciones->param3, instrucciones->param3_length);

			list_add(nueva_lista, nueva_instruccion);
		}

	return nueva_lista;
}

t_contexto* inicializar_contexto(){
	t_contexto* contexto = malloc(sizeof(t_contexto));
	contexto->instrucciones = list_create();
	contexto->registros = inicializar_registros();
	contexto->param = malloc(sizeof(char)*2);
	memcpy(contexto->param,"0",(sizeof(char)*2));
	contexto->estado = EXIT;
	contexto->param_length = sizeof(char)*2;
	contexto->pid = 0;

	return contexto;
}

void destroy_proceso(pcb_t* proceso){
	list_destroy_and_destroy_elements(proceso->instrucciones, (void*)instrucciones_destroy);
	free(proceso->recurso_bloqueante);
	free(proceso->registros_cpu);
	free(proceso);
}
