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

int instancias_de_un_recurso(t_lista_mutex *lista_recursos, const char* nombre_recurso) {
	t_recurso* recurso = buscar_recurso(lista_recursos, nombre_recurso);
	return recurso->instancias;
}
