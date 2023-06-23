#ifndef CLIENT_UTILS_H_
#define CLIENT_UTILS_H_

#include<stdio.h>
#include<stdlib.h>
#include<unistd.h>
#include<netdb.h>
#include<commons/log.h>
#include<commons/collections/list.h>
#include "../../shared/includes/tad.h"
#include"calculos.h"
#include"datos.h"

void crear_header(void* a_enviar, t_buffer* buffer, uint32_t lineas);
void serializar_tabla_segmentos(int socket, t_list* lista_tablas);
void serializar_respuesta_memoria_kernel(int socket_cliente,t_resp_mem respuesta);
void serializar_instruccion_memoria(int socket,t_instruc_mem* instruccion);

#endif /* CLIENT_UTILS_H_ */
