#include "../includes/comm_threadKernel.h"

t_contexto* obtener_contexto_pcb(pcb_t *pcb) {

	t_contexto *contexto = malloc(sizeof(t_contexto));
	t_registros *registros = malloc(sizeof(t_registros));
	*registros = pcb->registros_cpu;
	contexto->registros = registros;
	contexto->instrucciones = pcb->instrucciones;
	contexto->pid = pcb->pid;
	return contexto;
}

void enviar_contexto(pcb_t *pcb) { // aca recibir un pcb (pbc_t pbc)
	t_contexto *contexto = obtener_contexto_pcb(pcb);
	t_paquete *paquete2 = malloc(sizeof(t_paquete));
	paquete2->buffer = malloc(sizeof(t_buffer));
	//crear contexto c on ese pcb
	//inicializo el estado en 0 ya que el CPU es quien nos va a responder con el estado correcto
	//contexto = obtener_contexto_pcb(pcb_t* pcb)
	contexto->estado = EXIT;
	contexto->param = "0";
	contexto->param_length = strlen(contexto->param) + 1;

	t_contexto *contexto_actualizado = malloc(sizeof(t_contexto));
	contexto_actualizado->instrucciones = list_create();
	serializar_contexto(cpu_connection, contexto);
	//esperar respuesta de cpu
	//Recibo el header del paquete + el stream de datos
	deserializar_header(paquete2,cpu_connection);
	switch(paquete2->codigo_operacion){
		case 1:
			contexto_actualizado = deserializar_contexto(paquete2->buffer, paquete2->lineas);
			//log_info(logger,contexto_actualizado->registros->ax);
			// aca va logica de exit

		log_info(logger, "El IP esta en %d",
				contexto_actualizado->registros->ip);
		log_info(logger, "El size de las instrucciones es %d",
				(uint16_t) list_size(contexto->instrucciones));
		log_info(logger, "El numero de estado es: %d",
				contexto_actualizado->estado);
		log_info(logger, "El parametro de interrupcion es: %s",
				contexto_actualizado->param);

		//esto hay que mejorarlo

		switch (contexto_actualizado->estado) {
		case EXIT:
			list_push(pcb_exit_list, pcb);
			sem_post(&sem_estado_exit);
			break;
		case YIELD:
			//va devuelta a ready al final de la cola
			break;
		case IO:
			//crear un hilo por cada uno, que espere el tiempo de sleep y despues vuelva a ready
			break;
		case WAIT:
			char *recurso_wait = contexto_actualizado->param;

			if (recurso_existe_en_lista(lista_recursos, recurso_wait)) {

				restar_instancia(lista_recursos, recurso_wait);

				int instancias_recurso = instancias_de_un_recurso(lista_recursos, recurso_wait);

				if (instancias_recurso < 0) {

					pcb->recurso_bloqueante = recurso_wait;

					list_push(pcb_block_list, pcb);

					sem_post(&sem_estado_block);
				}
			} else {
				list_push(pcb_exit_list, pcb);

				sem_post(&sem_estado_exit);
			}
			break;
		case SIGNAL:
			char *recurso_signal = contexto_actualizado->param;

			if (recurso_existe_en_lista(lista_recursos, recurso_signal)) {

				sumar_instancia(lista_recursos, recurso_signal);

				int instancias_recurso = instancias_de_un_recurso(lista_recursos, recurso_signal);

				if(instancias_recurso > 0){ // revisar

					t_recurso* recurso_bloqueante = buscar_recurso(lista_recursos, recurso_signal);

					pcb_t* pcb_desbloqueado = list_pop(recurso_bloqueante->cola_bloqueados);

					list_push(pcb_ready_list, pcb_desbloqueado);

					sem_post(&sem_estado_ready);
				}
			} else {
				list_push(pcb_exit_list, pcb);

				sem_post(&sem_estado_exit);
			}
			break;
		case FSYSTEM:
			//revisar el recurso
			//si el recurso no esta disponible, agregar al pcb el nombre en recurso_bloqueante
			//y pushear a block
			//Si no existe o existe y esta disponible ejecutar logica de FILE
			//no hace falta meterlo en block
		case MEM:
			//revisar el recurso
			//si el recurso no esta disponible, agregar al pcb el nombre en recurso_bloqueante
			//y pushear a block
			//Si no existe o existe y esta disponible ejecutar logica de FILE
			//no hace falta meterlo en block
		default:

			//aca iria logica de bloq
			break;
		}

		//Esto deberia ir dentro del If que analizaria si va a Exit
		//pcb_t *proceso_a_exit = crear_proceso(contexto_actualizado->instrucciones);
		//list_push(pcb_exit_list,proceso_a_exit);
		//sem_post(&sem_estado_exit);

		// Hasta linea 49
		if (contexto_actualizado->registros->ip
				== (uint16_t) list_size(contexto->instrucciones)) {
			log_info(logger, "El contexto se ejecutó completamente");

			list_destroy_and_destroy_elements(
					contexto_actualizado->instrucciones, free);
			free(contexto_actualizado->registros);
			free(contexto_actualizado);

		}
		break;
	default:
		log_info(logger, "falle");
		break;
	}

}





