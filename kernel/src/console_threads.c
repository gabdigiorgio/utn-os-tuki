/*
 * threads.c
 *
 *  Created on: Apr 7, 2023
 *      Author: utnso
 */

#include "../includes/console_threads.h"
#include "../includes/server_utils.h"

void atender_consola(int socket_servidor){
	t_list* instruc_lista;
	instruc_lista = list_create();

	while (1){

		int socket_console_client = 0;
		socket_console_client = esperar_cliente(socket_servidor);

		//Reservo memoria para el paquete
		t_paquete* paquete = malloc(sizeof(t_paquete));
		paquete->buffer = malloc(sizeof(t_buffer));

		//Recivo el header del paquete + el stream de datos
		deserializar_header(paquete, socket_console_client);

		//Reviso el header para saber de que paquete se trata y deserealizo acorde
		switch(paquete->codigo_operacion)
		{
			case 1:
				instruc_lista = deserializar_instrucciones(paquete->buffer, paquete->lineas);
				break;
			default:
				break;
		}

		int lineas = list_size(instruc_lista);
		t_instruc* instrucciones = malloc(sizeof(t_instruc));

		for(int i = 0; i < lineas; i++)
		{
			instrucciones = list_get(instruc_lista, i);
			log_info(logger,"--------------------");
			log_info(logger,instrucciones->instruct);
			if(strcmp(instrucciones->param1,"")) log_info(logger,instrucciones->param1);
			if(strcmp(instrucciones->param2,"")) log_info(logger,instrucciones->param2);
			if(strcmp(instrucciones->param3,"")) log_info(logger,instrucciones->param3);
			log_info(logger,"--------------------");
		}
		agregar_pcb_a_new(socket_console_client,instruc_lista);
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

		registros->ip = 0;
		strcpy(registros->ax,"000a");
		strcpy(registros->bx,"000b");
		strcpy(registros->cx,"000c");
		strcpy(registros->dx,"000d");
		strcpy(registros->eax,"000000ea");
		strcpy(registros->ebx,"000000eb");
		strcpy(registros->ecx,"000000ec");
		strcpy(registros->edx,"000000ed");
		strcpy(registros->rax,"00000000000000ra");
		strcpy(registros->rbx,"00000000000000rb");
		strcpy(registros->rcx,"00000000000000rc");
		strcpy(registros->rdx,"00000000000000rd");

		contexto->instrucciones = list_create();

		contexto->instrucciones = instruc_lista;
		contexto->registros = registros;

		serializar_contexto(cpu_connection,contexto);

		free(paquete->buffer->stream);
		free(paquete->buffer);
		free(paquete);

		sleep(10);
	}
}
