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
				agregar_pcb_a_new(instruc_lista);
				break;
			default:
				break;
		}
	}
}
