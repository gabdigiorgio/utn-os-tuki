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

#include "general_planification_utils.h"

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
char* crear_recurso(const char *nombre_recurso);
t_recurso* buscar_recurso(t_lista_mutex*lista_recursos, const char *nombre_recurso);
bool recurso_existe_en_lista(t_lista_mutex* lista_recursos, const char* nombre_recurso);
void restar_instancia(t_lista_mutex* lista_recursos, const char *nombre_recurso);
void sumar_instancia(t_lista_mutex* lista_recursos, const char *nombre_recurso);
int obtener_instancias(t_lista_mutex* lista_recurso, const char* nombre_recurso);
void asignar_recurso(pcb_t *pcb, const char *nombre_recurso);
void desasignar_recurso_si_lo_tiene_asignado(pcb_t *pcb, const char* nombre_recurso);
void devolver_instancias(pcb_t* pcb, t_lista_mutex *lista_recursos);
void liberar_proceso_de_bloqueados_si_necesario(const char* recurso, int instancias_recurso);
void copiar_instruccion_mem(t_instruc_mem* instruccion, t_contexto* contexto);

char* armar_lista_pids(t_list* lista);
void instrucciones_destroy(t_instruc* instruccion);
t_registros* inicializar_registros(void);
void copiar_registros(t_registros* to_registros, t_registros* from_registros);
t_list* copiar_lista_instrucciones(t_list* nueva_lista, t_list* lista_instrucciones);
t_contexto* inicializar_contexto();
void destroy_proceso(pcb_t* proceso);
t_instruc_mem* inicializar_instruc_mem();
void copiar_instruccion_mem(t_instruc_mem* instruccion, t_contexto* contexto);
tabla_segmentos_t* buscar_tabla_segmentos(t_list* lista_tablas, int pid);
bool existe_tabla_segmentos(t_list* lista_tablas, int pid);

#endif /* CLIENT_H_ */
