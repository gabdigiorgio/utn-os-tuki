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

t_log* iniciar_logger(void);
t_config* iniciar_config(char * path_config);

// funciones de lista mutex
t_lista_mutex* init_list_mutex();
void list_push(t_lista_mutex * list , void* info);
void * list_pop(t_lista_mutex * list);
void list_mutex_destroy(t_lista_mutex * list);
bool list_mutex_is_empty(t_lista_mutex * list);
int list_mutex_size(t_lista_mutex * list);

// funciones de recursos
t_recurso* buscar_recurso(t_lista_mutex*lista_recursos, const char *nombre_recurso);
bool recurso_existe_en_lista(t_lista_mutex* lista_recursos, const char* nombre_recurso);
void restar_instancia(t_lista_mutex* lista_recursos, const char *nombre_recurso);
void sumar_instancia(t_lista_mutex* lista_recursos, const char *nombre_recurso);
int obtener_instancias(t_lista_mutex* lista_recurso, const char* nombre_recurso);

#endif /* CLIENT_H_ */
