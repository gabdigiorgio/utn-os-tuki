#ifndef UTILS_H_
#define UTILS_H_

#include<stdio.h>
#include<stdlib.h>
#include<commons/log.h>
#include<commons/string.h>
#include<commons/config.h>
#include<readline/readline.h>
#include "../../shared/includes/tad.h"

t_log* iniciar_logger(void);
t_config* iniciar_config(char * path_config);
void copiar_instruccion_mem(t_instruc_mem* instruccion, t_contexto* contexto);
t_instruc_mem* inicializar_instruc_mem();
void reducir_huecos(t_list* lista_huecos);
void imprimir_valores_huecos(hueco_libre_t* hueco);
void imprimir_valores_segmentos(segmento_t* segmento);
bool ordenar_lista_huecos(hueco_libre_t* hueco1, hueco_libre_t* hueco2);
void compactar_memoria(t_list* lista_segmentos, t_list* lista_huecos);
bool ordenar_lista_segmentos(segmento_t* segmento1, segmento_t* segmento2);
#endif /* CLIENT_H_ */

extern t_log* logger;
