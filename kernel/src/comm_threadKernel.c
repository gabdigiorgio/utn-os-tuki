#include "../includes/comm_threadKernel.h"

t_contexto* obtener_contexto_pcb(pcb_t* pcb) {

	t_contexto *contexto = malloc(sizeof(t_contexto));
	t_registros *registros = malloc(sizeof(t_registros));
	contexto->registros = registros;
	contexto->instrucciones = pcb->instrucciones;
	return contexto;
}

void enviar_contexto(t_contexto* contexto){
	t_paquete* paquete2 = malloc(sizeof(t_paquete));
	paquete2->buffer = malloc(sizeof(t_buffer));
	t_contexto* contexto_actualizado = malloc(sizeof(t_contexto));
	contexto_actualizado->instrucciones = list_create();
	serializar_contexto(cpu_connection,contexto);
	//esperar respuesta de cpu
	//Recibo el header del paquete + el stream de datos
	deserializar_header(paquete2,cpu_connection);
	switch(paquete2->codigo_operacion){
		case 1:
			contexto_actualizado = deserializar_contexto(paquete2->buffer, paquete2->lineas);
			log_info(logger,contexto_actualizado->registros->ax);
			break;
		default:
			log_info(logger,"falle");
			break;
	}

	// aca va logica de exit

	log_info(logger, "El IP esta en %d", contexto_actualizado->registros->ip);
	log_info(logger, "El size de las instrucciones es %d", (uint16_t)list_size(contexto->instrucciones));

	if(contexto_actualizado->registros->ip == (uint16_t)list_size(contexto->instrucciones))
	{
		log_info(logger,"El contexto se ejecutó completamente");

	    list_destroy_and_destroy_elements(contexto_actualizado->instrucciones, free);
	    free(contexto_actualizado->registros);
	    free(contexto_actualizado);
	}
	/*
	else
	{
		log_info(logger,"proceso a block");

		if(bloqueado_por_io(pcb))
		{
			// no se encola
			// obtener tiempo bloqueado por I/O por medio de la CPU al devolver el contexto de ejecucion
		}

		char* recurso_solicitado = obtener_recurso(contexto_actualizado);

		else if(desplazado_por_wait(contexto_actualizado))
		{
			if(existe_recurso(recurso_solicitado)) // los recursos vienen dados en el .config
			{
				restar_instancia(instancias_recursos, recurso_solicitado);

				if(instancias_de_recurso(instancias_recursos, recurso_solicitado) < 0)
				{
					list_push(pcb_block_list, pcb);
				}
			}
			else
			{
				enviar_proceso_a_exit(pcb);
			}
		}
		else if(desplazado_por_signal(contexto_actualizado))
		{
			if(existe_recurso(recurso_solicitado))
			{
				sumar_instancia(instancias_recursos, recurso_solicitado);
				desbloquear_primer_proceso_bloqueado(recurso_solicitado);
				enviar_proceso_a_ejecutar(pcb);
			}
			else
			{
				enviar_proceso_a_exit(pcb);
			}
		}
	}*/

}
