/*
 * threads.c
 *
 *  Created on: Apr 7, 2023
 *      Author: utnso
 */

#include "../includes/console_threads.h"
#include "../includes/server_utils.h"

void atender_consola(int *socket_console_client){
	t_list* instruc_lista = malloc(sizeof(t_list));
	int socket = *socket_console_client;

	instruc_lista = list_create();

	int estado = 1;
	log_info(logger, "Thread iniciado correctamente");
	while (estado == 1) {
		//Reservo memoria para el paquete
		t_paquete* paquete = malloc(sizeof(t_paquete));
		paquete->buffer = malloc(sizeof(t_buffer));

		//Recivo el header del paquete + el stream de datos
		deserializar_header(paquete, socket);

		//Reviso el header para saber de que paquete se trata y deserealizo acorde
		switch(paquete->codigo_operacion){
			case 1:
				instruc_lista = deserializar_instrucciones(paquete->buffer, paquete->lineas);
				break;
			default:
				break;
		}

		int lineas = list_size(instruc_lista);
		t_instruc* instrucciones = malloc(sizeof(t_instruc));

		for(int i = 0; i < lineas; i++){
					instrucciones = list_get(instruc_lista, i);
					log_info(logger,"--------------------");
					log_info(logger,instrucciones->instruct);
					if(strcmp(instrucciones->param1,"")) log_info(logger,instrucciones->param1);
					if(strcmp(instrucciones->param2,"")) log_info(logger,instrucciones->param2);
					if(strcmp(instrucciones->param3,"")) log_info(logger,instrucciones->param3);
					log_info(logger,"--------------------");
				}

		t_contexto* contexto = malloc(sizeof(t_contexto));
		t_registros* registros = malloc(sizeof(t_registros));

		char adsa[5] = "0004";

		registros->ip = 0;
		registros->ax = "000a";
		registros->bx = "000b";
		registros->cx = "000c";
		registros->dx = "000d";
		registros->eax = "00ea";
		registros->ebx = "00eb";
		registros->ecx = "00ec";
		registros->edx = "00ed";
		registros->rax = "00ra";
		registros->rbx = "00rb";
		registros->rcx = "00rc";
		registros->rdx = "00rd";

		contexto->instrucciones = instrucciones;
		contexto->registros = registros;

		serializar_contexto(cpu_connection,contexto);

		free(paquete->buffer->stream);
		free(paquete->buffer);
		free(paquete);
		}
}
