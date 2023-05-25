#ifndef UTILS_H_
#define UTILS_H_

#include "../../shared/includes/tad.h"
#include<stdio.h>
#include<stdlib.h>
#include<stdbool.h>
#include<commons/log.h>
#include<commons/string.h>
#include<commons/config.h>
#include<readline/readline.h>
#include <commons/collections/list.h>
#include <pthread.h>

typedef struct {
	t_list * lista;
    pthread_mutex_t mutex;
} t_lista_mutex;

t_log* iniciar_logger(void);
t_config* iniciar_config(char * path_config);
t_lista_mutex* init_list_mutex();
void list_push(t_lista_mutex * list , void* info);
void * list_pop(t_lista_mutex * list);
void list_mutex_destroy(t_lista_mutex * list);
bool list_mutex_is_empty(t_lista_mutex * list);
int list_mutex_size(t_lista_mutex * list);
t_recurso* buscar_recurso(t_list *lista_recursos, const char *nombre_recurso);
bool recurso_existe_en_lista(t_list* lista_recursos, const char* nombre_recurso);
void restar_instancia(t_list *lista_recursos, const char *nombre_recurso);
void sumar_instancia(t_list *lista_recursos, const char *nombre_recurso);
int instancias_de_un_recurso(t_list *lista_recurso, const char* nombre_recurso);
#endif /* CLIENT_H_ */
