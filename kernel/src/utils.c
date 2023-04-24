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
	t_log* nuevo_logger = log_create("kernel.log", "KERNEL", 1, LOG_LEVEL_DEBUG);

	return nuevo_logger;
}

t_lista_mutex* init_list_mutex() {
	t_lista_mutex* list = malloc(sizeof(t_lista_mutex));

	list ->lista = list_create();
    pthread_mutex_init(&(list->mutex), NULL);
    return list;
}


void list_push(t_lista_mutex * list , void* info) {
    pthread_mutex_lock(&(list->mutex));
    list_add(list->lista, info);
    pthread_mutex_unlock(&(list->mutex));
}


void * list_pop(t_lista_mutex * list){
	pthread_mutex_lock(&(list->mutex));
	void* info = list_remove(list->lista, 0);
	pthread_mutex_unlock(&(list->mutex));
	return info;
}

void list_mutex_destroy(t_lista_mutex * list){
	list_destroy(list->lista);
	pthread_mutex_destroy(&(list->mutex));
	free(list);
}

