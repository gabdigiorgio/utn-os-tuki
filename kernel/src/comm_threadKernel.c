#include "../includes/comm_threadKernel.h"

t_contexto* obtener_contexto_pcb(pcb_t* pcb) {

	t_contexto *contexto = malloc(sizeof(t_contexto));
	t_registros *registros = malloc(sizeof(t_registros));
	contexto->registros = registros; //esto esta mal, tiene que tomarlo del PCB
	contexto->instrucciones = pcb->instrucciones;
	contexto->pid = pcb->pid;
	return contexto;
}

void enviar_contexto(t_contexto* contexto){ // aca recibir un pcb
	t_paquete* paquete2 = malloc(sizeof(t_paquete));
	paquete2->buffer = malloc(sizeof(t_buffer));
    //crear contexto c on ese pcb
	//inicializo el estado en 0 ya que el CPU es quien nos va a responder con el estado correcto
	contexto->estado = EXIT;
	contexto->param = "0";
	contexto->param_length = strlen(contexto->param) + 1;


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
			// aca va logica de exit

				log_info(logger, "El IP esta en %d", contexto_actualizado->registros->ip);
				log_info(logger, "El size de las instrucciones es %d", (uint16_t)list_size(contexto->instrucciones));
				log_info(logger, "El numero de estado es: %d", contexto_actualizado->estado);
				log_info(logger, "El parametro de interrupcion es: %s", contexto_actualizado->param);

				pcb_t *proceso_a_exit = malloc(sizeof(pcb_t)); // utlizar el mismo PCB de antes
					proceso_a_exit->pid = contexto_actualizado->pid;
					proceso_a_exit->estimado_proxima_rafaga = 0;
					proceso_a_exit->instrucciones = contexto_actualizado->instrucciones;

				switch(contexto_actualizado->estado){
					case EXIT:
						list_push(pcb_exit_list,proceso_a_exit);
						sem_post(&sem_estado_exit);
						break;
					default:
						break;
				}

				//Esto deberia ir dentro del If que analizaria si va a Exit
				//pcb_t *proceso_a_exit = crear_proceso(contexto_actualizado->instrucciones);
				//list_push(pcb_exit_list,proceso_a_exit);
				//sem_post(&sem_estado_exit);

				// Hasta linea 49
				if(contexto_actualizado->registros->ip == (uint16_t)list_size(contexto->instrucciones))
				{
					log_info(logger,"El contexto se ejecutó completamente");

				    list_destroy_and_destroy_elements(contexto_actualizado->instrucciones, free);
				    free(contexto_actualizado->registros);
				    free(contexto_actualizado);


				}
			break;
		default:
			log_info(logger,"falle");
			break;
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
