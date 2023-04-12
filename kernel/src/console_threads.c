/*
 * threads.c
 *
 *  Created on: Apr 7, 2023
 *      Author: utnso
 */

#include "../includes/console_threads.h"

void atender_consola(int *socket_console_client){
	t_list* instruc_lista;
	int socket = *socket_console_client;

	instruc_lista = list_create();

	int estado = 1;
	log_info(logger, "Thread iniciado correctamente");
	while (estado == 1) {
		t_paquete* paquete = malloc(sizeof(t_paquete));
		paquete->buffer = malloc(sizeof(t_buffer));

		if((recv(socket, &(paquete->codigo_operacion), sizeof(uint32_t), 0)) > 0){
			printf("recibi algo");
		}
		if((recv(socket, &(paquete->lineas), sizeof(uint32_t), 0)) > 0){
			printf("recibi algo");
		}
		if((recv(socket, &(paquete->buffer->size), sizeof(uint32_t), 0)) > 0){
			printf("recibi algo");
		}
		paquete->buffer->stream = malloc(paquete->buffer->size);
		if((recv(socket, paquete->buffer->stream, paquete->buffer->size, 0)) > 0){
			printf("recibi algo");
		}

		switch(paquete->codigo_operacion){
			case 1:
				instruc_lista = deserializar_instrucciones(paquete->buffer, paquete->lineas);
				break;
			default:
				break;
		}

		free(paquete->buffer->stream);
		free(paquete->buffer);
		free(paquete);
		}
}

t_list* deserializar_instrucciones(t_buffer* buffer, int lineas){
	t_list* lista = malloc(sizeof(t_list));

	void* stream = buffer->stream;

	for(int i=0; i<lineas; i++){
		t_instruc* instrucciones = malloc(sizeof *instrucciones);

		memcpy(&(instrucciones->nro), stream, sizeof(uint32_t));
		stream += sizeof(uint32_t);

		memcpy(&(instrucciones->instruct_length), stream, sizeof(uint32_t));
		stream += sizeof(uint32_t);

		instrucciones->instruct = malloc(instrucciones->instruct_length);
		memcpy(instrucciones->instruct, stream, instrucciones->instruct_length);
		stream += instrucciones->instruct_length;

		memcpy(&(instrucciones->param1_length), stream, sizeof(uint32_t));
		stream += sizeof(uint32_t);

		instrucciones->param1 = malloc(instrucciones->param1_length);
		memcpy(instrucciones->param1, stream, instrucciones->param1_length);
		stream += instrucciones->param1_length;

		memcpy(&(instrucciones->param2_length), stream, sizeof(uint32_t));
		stream += sizeof(uint32_t);

		instrucciones->param2 = malloc(instrucciones->param2_length);
		memcpy(instrucciones->param2, stream, instrucciones->param2_length);
		stream += instrucciones->param2_length;

		memcpy(&(instrucciones->param3_length), stream, sizeof(uint32_t));
		stream += sizeof(uint32_t);

		instrucciones->param3 = malloc(instrucciones->param3_length);
		memcpy(instrucciones->param3, stream, instrucciones->param3_length);
		stream += instrucciones->param3_length;

		list_add(lista, instrucciones);
	}

	return lista;
}
