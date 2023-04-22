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
		free(paquete->buffer->stream);
		free(paquete->buffer);
		free(paquete);

		sleep(10);
	}
}
