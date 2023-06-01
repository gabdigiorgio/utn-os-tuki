#include "../includes/comm_threadKernel.h"

t_contexto* obtener_contexto_pcb(pcb_t *pcb)
{

	t_contexto *contexto = inicializar_contexto();
	copiar_registros(contexto->registros, pcb->registros_cpu);
	copiar_lista_instrucciones(contexto->instrucciones, pcb->instrucciones);
	contexto->pid = pcb->pid;
	return contexto;
}

void enviar_contexto(pcb_t *pcb)
{
	t_contexto *contexto = obtener_contexto_pcb(pcb);
	t_paquete *paquete = malloc(sizeof(t_paquete));
	paquete->buffer = malloc(sizeof(t_buffer));

	serializar_contexto(cpu_connection, contexto);
	deserializar_header(paquete, cpu_connection);
	switch (paquete->codigo_operacion)
	{
	case 1:
		t_contexto *contexto_actualizado = inicializar_contexto();
		deserializar_contexto(contexto_actualizado, paquete->buffer, paquete->lineas);
		copiar_registros(pcb->registros_cpu, contexto_actualizado->registros);

		switch (contexto_actualizado->estado)
		{
		case EXIT:
			list_push(pcb_exit_list, pcb);
			sem_post(&sem_estado_exit);
			break;

		case YIELD:
			list_push(pcb_ready_list, pcb);
			pcb->tiempo_espera_en_ready = temporal_create();
			log_info(logger, "El proceso %d llego a yield. Se envio al final de ready", pcb->pid);
			sem_post(&sem_estado_ready);
			break;

		case IO:
			t_io_block_args *args = malloc(sizeof(t_io_block_args));
			args->pcb = pcb;
			int time = atoi(contexto_actualizado->param);
			args->block_time = time;
			pthread_t thread_io_block;
			pthread_create(&thread_io_block, NULL, (void*) io_block, (t_io_block_args*) args);
			pthread_detach(thread_io_block);
			break;

		case WAIT:
			char *recurso_wait = crear_recurso(contexto_actualizado->param);

			if (recurso_existe_en_lista(lista_recursos, recurso_wait))
			{
				asignar_recurso(pcb, recurso_wait);
				restar_instancia(lista_recursos, recurso_wait);
				int instancias_recurso = obtener_instancias(lista_recursos, recurso_wait);
				log_info(logger, "PID: %d - Wait: %s - Instancias: %d", pcb->pid, recurso_wait, instancias_recurso);
				if (instancias_recurso < 0)
				{
					int recurso_length = strlen(recurso_wait);
					pcb->recurso_bloqueante = realloc(pcb->recurso_bloqueante, recurso_length);
					memcpy(pcb->recurso_bloqueante, recurso_wait, recurso_length);
					list_push(pcb_block_list, pcb);
					sem_post(&sem_estado_block);
				}
				else
				{
					enviar_contexto(pcb);
				}
			}
			else
			{
				list_push(pcb_exit_list, pcb);
				sem_post(&sem_estado_exit);
			}

			free(recurso_wait);
			break;

		case SIGNAL:
			char *recurso_signal = crear_recurso(contexto_actualizado->param);
			if (recurso_existe_en_lista(lista_recursos, recurso_signal))
			{
				desasignar_recurso_si_lo_tiene_asignado(pcb, recurso_signal);
				sumar_instancia(lista_recursos, recurso_signal);
				int instancias_recurso = obtener_instancias(lista_recursos, recurso_signal);
				log_info(logger, "PID: %d - Signal: %s - Instancias: %d", pcb->pid, recurso_signal, instancias_recurso);
				enviar_contexto(pcb);
				liberar_proceso_de_bloqueados_si_necesario(recurso_signal, instancias_recurso);
			}
			else
			{
				list_push(pcb_exit_list, pcb);
				sem_post(&sem_estado_exit);
			}
			free(recurso_signal);
			break;

		case FSYSTEM:
			//revisar el recurso
			//si el recurso no esta disponible, agregar al pcb el nombre en recurso_bloqueante
			//y pushear a block
			//Si no existe o existe y esta disponible ejecutar logica de FILE
			//no hace falta meterlo en block
			break;

		case MEM:
			//revisar el recurso
			//si el recurso no esta disponible, agregar al pcb el nombre en recurso_bloqueante
			//y pushear a block
			//Si no existe o existe y esta disponible ejecutar logica de FILE
			//no hace falta meterlo en block
			break;

		default:
			//aca iria logica de bloq
			break;
		}

		list_destroy_and_destroy_elements(contexto_actualizado->instrucciones, (void*) instrucciones_destroy);
		free(contexto_actualizado->param);
		free(contexto_actualizado->registros);
		free(contexto_actualizado);
		break;

	default:
		log_info(logger, "falle");
		break;
	}

	list_destroy_and_destroy_elements(contexto->instrucciones, (void*) instrucciones_destroy);
	free(contexto->param);
	free(contexto->registros);
	free(contexto);
	free(paquete->buffer->stream);
	free(paquete->buffer);
	free(paquete);

}
