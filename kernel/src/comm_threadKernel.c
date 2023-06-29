#include "../includes/comm_threadKernel.h"

t_contexto* obtener_contexto_pcb(pcb_t *pcb)
{

	t_contexto *contexto = inicializar_contexto();
	copiar_registros(contexto->registros, pcb->registros_cpu);
	copiar_lista_instrucciones(contexto->instrucciones, pcb->instrucciones);
	copiar_tabla_segmentos(contexto->tabla_segmento, pcb->tabla_segmento);
	contexto->pid = pcb->pid;
	return contexto;
}

contexto_estado_t enviar_contexto(pcb_t *pcb)
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
			log_info(logger, "PID: %d - Estado Anterior: PCB_EXEC - Estado Actual: PCB_EXIT", pcb->pid);
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
			int time = atoi(contexto_actualizado->param1);
			args->block_time = time;
			pthread_t thread_io_block;
			pthread_create(&thread_io_block, NULL, (void*) io_block, (t_io_block_args*) args);
			pthread_detach(thread_io_block);
			break;

		case WAIT:
			char *recurso_wait = crear_recurso(contexto_actualizado->param1);

			if (recurso_existe_en_lista(lista_recursos, recurso_wait))
			{
				asignar_recurso(pcb, recurso_wait);
				restar_instancia(lista_recursos, recurso_wait);
				int instancias_recurso = obtener_instancias(lista_recursos, recurso_wait);
				log_info(logger, "PID: %d - Wait: %s - Instancias: %d", pcb->pid, recurso_wait, instancias_recurso);
				if (instancias_recurso < 0)
				{
					int recurso_length = strlen(recurso_wait) + 1;
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
				log_error(logger, "No existe el recurso %s - terminando proceso PID: %d", recurso_wait, pcb->pid);
				sem_post(&sem_estado_exit);
			}

			free(recurso_wait);
			break;

		case SIGNAL:
			char *recurso_signal = crear_recurso(contexto_actualizado->param1);
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
				log_error(logger, "No existe el recurso %s - terminando proceso PID: %d", recurso_signal, pcb->pid);
				sem_post(&sem_estado_exit);
			}
			free(recurso_signal);
			break;

		case F_OPEN:

			char *archivo_a_abrir = crear_recurso(contexto_actualizado->param1);

			archivo_abierto_t *archivo = malloc(sizeof(archivo_abierto_t));;
			archivo->nombre_archivo = crear_recurso(contexto_actualizado->param1);;
			archivo->posicion_puntero = "0";

			list_add(pcb->tabla_archivos_abiertos, archivo);

			if(!archivo_existe_en_tabla(tabla_global_archivos_abiertos, archivo_a_abrir)){

				list_add(tabla_global_archivos_abiertos, archivo_a_abrir);


				int recurso_length = strlen(archivo_a_abrir) + 1;
				t_recurso *recurso = malloc(sizeof(t_recurso));
				recurso->id = list_size(lista_recursos->lista);
				recurso->nombre_recurso = malloc(recurso_length);
				memcpy(recurso->nombre_recurso,archivo_a_abrir,recurso_length);
				recurso->instancias = 1;
				pthread_mutex_init(&(recurso->mutex_instancias), NULL);
				recurso->cola_bloqueados = init_list_mutex();
				list_add(lista_recursos->lista, recurso);

			}


			manejar_archivo(contexto_actualizado,pcb);

			asignar_recurso(pcb, archivo_a_abrir);
			restar_instancia(lista_recursos, archivo_a_abrir);

			int instancias_recurso = obtener_instancias(lista_recursos, archivo_a_abrir);
			if (instancias_recurso < 0)
			{
				int recurso_length = strlen(archivo_a_abrir) + 1;
				pcb->recurso_bloqueante = realloc(pcb->recurso_bloqueante, recurso_length);
				memcpy(pcb->recurso_bloqueante, archivo_a_abrir, recurso_length);
				list_push(pcb_block_list, pcb);
				sem_post(&sem_estado_block);
			}
			else
			{
				enviar_contexto(pcb);
			}


			break;

		case F_DELETE:
			manejar_archivo(contexto_actualizado,pcb);
			log_info(logger, "El proceso %d se comunico con FileSystem. Se continua su ejecucion", pcb->pid);
			enviar_contexto(pcb);
			break;

		case F_CLOSE:

			char *archivo_abierto = contexto_actualizado->param1;

			if (recurso_existe_en_lista(lista_recursos, archivo_abierto) && archivo_existe_en_tabla(tabla_global_archivos_abiertos, archivo_abierto))
			{
				desasignar_recurso_si_lo_tiene_asignado(pcb, archivo_abierto);
				sumar_instancia(lista_recursos, archivo_abierto);
				int instancias_recurso = obtener_instancias(lista_recursos, archivo_abierto);

				manejar_archivo(contexto_actualizado,pcb);

				liberar_proceso_de_bloqueados_si_necesario(archivo_abierto, instancias_recurso);

				archivo_abierto_t* archivo_abierto_pcb = buscar_archivo_abierto_t(pcb->tabla_archivos_abiertos, archivo_abierto);
				list_remove_element(pcb->tabla_archivos_abiertos,archivo_abierto_pcb);

				if(instancias_recurso==1){
					eliminar_archivo_abierto_t(tabla_global_archivos_abiertos, archivo_abierto);
					t_recurso *recurso_a_eliminar = buscar_recurso(lista_recursos, archivo_abierto);
					list_mutex_destroy(recurso_a_eliminar->cola_bloqueados);
					pthread_mutex_destroy(&(recurso_a_eliminar->mutex_instancias));
					list_remove_element(lista_recursos->lista, recurso_a_eliminar);

				}

				enviar_contexto(pcb);
			}
			else
			{
				list_push(pcb_exit_list, pcb);
				log_error(logger, "No existe el archivo %s - terminando proceso PID: %d", archivo_abierto, pcb->pid);
				sem_post(&sem_estado_exit);
			}

			//cambiar por la correcta
			break;

		case F_TRUNCATE:
			//logica temporal hasta tener la que va
			//pthread_t thread_f_block;
			//pthread_create(&thread_f_block, NULL, (void*) f_block, (t_f_block_args*) args);
			//pthread_join(thread_f_block);
			manejar_archivo(contexto_actualizado,pcb);
			log_info(logger, "El proceso %d se comunico con FileSystem. Se continua su ejecucion", pcb->pid);
			enviar_contexto(pcb);
			//cambiar por la correcta
			break;

		case F_SEEK:

			char *archivo_abierto_seek = contexto_actualizado->param1;

			log_info(logger, "Tamanio %d", strlen(archivo_abierto_seek));
			archivo_abierto_t* archivo_abierto_seek_pcb = buscar_archivo_abierto_t(pcb->tabla_archivos_abiertos, archivo_abierto_seek);

			archivo_abierto_seek_pcb->posicion_puntero = copiar_char_puntero(contexto_actualizado->param2);

			log_info(logger, "Puntero %s", archivo_abierto_seek_pcb->posicion_puntero);

			manejar_archivo(contexto_actualizado,pcb);
			enviar_contexto(pcb);
			break;


		case F_READ:

			t_read_write_block_args *args_read = malloc(sizeof(t_read_write_block_args));
			args_read->pcb = pcb;
			args_read->contexto = inicializar_contexto();

			duplicar_contexto(args_read->contexto,contexto_actualizado);

			pthread_t thread_read_block;
			pthread_create(&thread_read_block, NULL, (void*) file_system_read_write_block, args_read);
			pthread_detach(thread_read_block);

			break;

		case F_WRITE:
			t_read_write_block_args *args_write = malloc(sizeof(t_read_write_block_args));
			args_write->pcb = pcb;
			args_write->contexto = inicializar_contexto();

			duplicar_contexto(args_write->contexto,contexto_actualizado);

			pthread_t thread_write_block;
			pthread_create(&thread_write_block, NULL, (void*) file_system_read_write_block, args_write);
			pthread_detach(thread_write_block);
			break;
		case PRINT_FILE_DATA:
			{
				t_instruc_file* instruccion = inicializar_instruc_file();
				char* puntero = string_new();
				string_append(&puntero,"");
				copiar_instruccion_file(instruccion,contexto_actualizado,puntero);
				serializar_instruccion_file(file_system_connection, instruccion);
				enviar_contexto(pcb);
			}
			break;
		case PRINT_FILE_STATE:
			imprimir_tabla_archivos();
			enviar_contexto(pcb);
			break;
		case CREATE_SEGMENT:
			log_info(logger, "PID: %d - Comunicacion con MEMORIA", pcb->pid);
			create_segment(contexto_actualizado,pcb);
			break;

		case DELETE_SEGMENT:
			log_info(logger, "PID: %d - Comunicacion con MEMORIA", pcb->pid);
			delete_segment(contexto_actualizado,pcb);
			solicitar_tabla_segmentos();
			enviar_contexto(pcb);
			break;
		case PRINT_SEGMENTS:
			{
				log_info(logger, "PID: %d - Comunicacion con MEMORIA", pcb->pid);
				t_instruc_mem* instruccion = inicializar_instruc_mem();
				copiar_instruccion_mem(instruccion,contexto_actualizado);
				serializar_instruccion_memoria(memoria_connection, instruccion);
				imprimir_tabla_segmentos();
			}
			enviar_contexto(pcb);
			break;
		case PRINT_MEMORY_DATA:
			{
				log_info(logger, "PID: %d - Comunicacion con MEMORIA", pcb->pid);
				t_instruc_mem* instruccion = inicializar_instruc_mem();
				copiar_instruccion_mem(instruccion,contexto_actualizado);
				serializar_instruccion_memoria(memoria_connection, instruccion);
			}
			enviar_contexto(pcb);
			break;
		default:
			//aca iria logica de bloq
			break;
		}

		list_destroy_and_destroy_elements(contexto_actualizado->instrucciones, (void*) instrucciones_destroy);
		free(contexto_actualizado->param1);
		free(contexto_actualizado->param2);
		free(contexto_actualizado->param3);
		free(contexto_actualizado->registros);
		free(contexto_actualizado);
		break;

	default:
		log_error(logger, "ERROR COMUNICACION CON CPU");
		break;
	}

	list_destroy_and_destroy_elements(contexto->instrucciones, (void*) instrucciones_destroy);
	free(contexto->param1);
	free(contexto->param2);
	free(contexto->param3);
	free(contexto->registros);
	free(contexto);
	free(paquete->buffer->stream);
	free(paquete->buffer);
	free(paquete);

	return EXEC;
}
