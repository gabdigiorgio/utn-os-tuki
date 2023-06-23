#ifndef UTILS_H_
#define UTILS_H_

#include<stdio.h>
#include<stdlib.h>
#include<commons/log.h>
#include<commons/string.h>
#include<commons/config.h>
#include<readline/readline.h>
#include "../../shared/includes/tad.h"
#include"calculos.h"
#include"datos.h"

extern t_list *lista_de_tablas;
extern int tam_memoria;
extern void* memoria;
extern t_log* logger;
extern t_list *lista_de_huecos_libres;

t_log* iniciar_logger(void);
t_config* iniciar_config(char * path_config);
void reducir_huecos(t_list* lista_huecos);
void imprimir_valores_huecos(hueco_libre_t* hueco);
void imprimir_valores_segmentos(segmento_t* segmento);
bool ordenar_lista_huecos(hueco_libre_t* hueco1, hueco_libre_t* hueco2);
void eliminar_segmento(tabla_segmentos_t* tabla_proceso, t_list* lista_huecos, uint32_t id_segmento);
void compactar_memoria();
bool ordenar_lista_segmentos(segmento_t* segmento1, segmento_t* segmento2);
int tam_memoria_restante();
t_list* extraer_segmentos();
void imprimir_tabla_segmentos();

#endif /* UTILS_H_ */
