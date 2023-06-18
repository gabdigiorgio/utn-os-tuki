#include "../includes/utils.h"

t_config* iniciar_config(char *path_config)
{
	t_config *nuevo_config;
	if ((nuevo_config = config_create(path_config)) == NULL)
	{
		printf("No pude leer la config \n");
		exit(2);
	}
	return nuevo_config;
}

t_log* iniciar_logger(void)
{
	t_log *nuevo_logger = log_create("kernel.log", "KERNEL", 1, LOG_LEVEL_DEBUG);

	return nuevo_logger;
}

t_lista_mutex* init_list_mutex()
{
	t_lista_mutex *list = malloc(sizeof(t_lista_mutex));

	list->lista = list_create();
	pthread_mutex_init(&(list->mutex), NULL);
	return list;
}

void list_push(t_lista_mutex *list, void *info)
{
	pthread_mutex_lock(&(list->mutex));
	list_add(list->lista, info);
	pthread_mutex_unlock(&(list->mutex));
}

void* list_pop(t_lista_mutex *list)
{
	pthread_mutex_lock(&(list->mutex));
	void *info = list_remove(list->lista, 0);
	pthread_mutex_unlock(&(list->mutex));
	return info;
}

void list_mutex_destroy(t_lista_mutex *list)
{
	list_destroy(list->lista);
	pthread_mutex_destroy(&(list->mutex));
	free(list);
}

bool list_mutex_is_empty(t_lista_mutex *list)
{
	pthread_mutex_lock(&(list->mutex));
	bool is_empty = list_is_empty(list->lista);
	pthread_mutex_unlock(&(list->mutex));
	return is_empty;
}

int list_mutex_size(t_lista_mutex *list)
{
	pthread_mutex_lock(&(list->mutex));
	int size = list_size(list->lista);
	pthread_mutex_unlock(&(list->mutex));
	return size;
}

void* list_mutex_get(t_lista_mutex *list, int index)
{
	pthread_mutex_lock(&(list->mutex));
	void *element = list_get(list->lista, index);
	pthread_mutex_unlock(&(list->mutex));
	return element;
}

char* crear_recurso(const char *param)
{
	char *string1 = malloc(sizeof(char) * 2);
	memcpy(string1, "0", (sizeof(char) * 2));

	string1 = realloc(string1, strlen(param) + 1);
	memcpy(string1, param, strlen(param) + 1);

	return string1;
}

// void borrar_recurso(const char* param)

t_recurso* buscar_recurso(t_lista_mutex *lista_recursos, const char *nombre_recurso)
{
	for (int i = 0; i < list_mutex_size(lista_recursos); i++)
	{
		t_recurso *recurso = (t_recurso*) list_mutex_get(lista_recursos, i);
		if (strcmp(recurso->nombre_recurso, nombre_recurso) == 0)
		{
			return recurso;
		}
	}
	return NULL;
}

bool recurso_existe_en_lista(t_lista_mutex *lista_recursos, const char *nombre_recurso)
{
	t_recurso *recurso = buscar_recurso(lista_recursos, nombre_recurso);
	return (recurso != NULL);
}

void restar_instancia(t_lista_mutex *lista_recursos, const char *nombre_recurso)
{
	t_recurso *recurso = buscar_recurso(lista_recursos, nombre_recurso);
	pthread_mutex_lock(&(recurso->mutex_instancias));
	recurso->instancias--;
	pthread_mutex_unlock(&(recurso->mutex_instancias));
}

void sumar_instancia(t_lista_mutex *lista_recursos, const char *nombre_recurso)
{
	t_recurso *recurso = buscar_recurso(lista_recursos, nombre_recurso);
	pthread_mutex_lock(&(recurso->mutex_instancias));
	recurso->instancias++;
	pthread_mutex_unlock(&(recurso->mutex_instancias));
}

int obtener_instancias(t_lista_mutex *lista_recursos, const char *nombre_recurso)
{
	t_recurso *recurso = buscar_recurso(lista_recursos, nombre_recurso);
	return recurso->instancias;
}

void asignar_recurso(pcb_t *pcb, const char *nombre_recurso)
{
	t_recurso *recurso_asignado = buscar_recurso(lista_recursos, nombre_recurso);
	list_add(pcb->recursos_asignados, recurso_asignado);
}

void desasignar_recurso_si_lo_tiene_asignado(pcb_t *pcb, const char *nombre_recurso)
{
	bool buscar_recurso(void *recurso){
		t_recurso *recurso_actual = (t_recurso *) recurso;
		return strcmp(recurso_actual->nombre_recurso, nombre_recurso) == 0;
	}

	list_remove_by_condition(pcb->recursos_asignados, buscar_recurso);
}

void devolver_instancias(pcb_t *pcb, t_lista_mutex *lista_recursos)
{
	if (!list_is_empty(pcb->recursos_asignados))
	{
		for (int i = 0; i < list_size(pcb->recursos_asignados); i++)
		{
			t_recurso *recurso_asignado = list_remove(pcb->recursos_asignados, i);
			sumar_instancia(lista_recursos, recurso_asignado->nombre_recurso);

			liberar_proceso_de_bloqueados_si_necesario(recurso_asignado->nombre_recurso, recurso_asignado->instancias);

			log_info(logger, "PID: %d - Libera recurso: %s - Instancias: %d", pcb->pid, recurso_asignado->nombre_recurso, recurso_asignado->instancias);
		}
	}

}

void liberar_proceso_de_bloqueados_si_necesario(const char *recurso, int instancias_recurso)
{
	if (instancias_recurso <= 0)
	{
		t_recurso *recurso_bloqueante = buscar_recurso(lista_recursos, recurso);
		pcb_t *pcb_desbloqueado = list_pop(recurso_bloqueante->cola_bloqueados);
		list_push(pcb_ready_list, pcb_desbloqueado);
		pcb_desbloqueado->tiempo_espera_en_ready = temporal_create();
		log_info(logger, "El proceso %d se libero de la cola de bloqueados", pcb_desbloqueado->pid);
		sem_post(&sem_estado_ready);
	}

}

char* armar_lista_pids(t_list *lista)
{
	int largo = list_size(lista);
	char *pids = string_new();
	string_append(&pids, "Cola Ready %s : ");

	for (int i = 0; i < largo; i++)
	{
		pcb_t *pcb = list_get(lista, i);
		char *pid = string_from_format("%u", pcb->pid);
		string_append(&pids, " ");
		string_append(&pids, pid);

		free(pid);
	}

	return pids;
}

void instrucciones_destroy(t_instruc *instruccion)
{
	free(instruccion->instruct);
	free(instruccion->param1);
	free(instruccion->param2);
	free(instruccion->param3);
	free(instruccion);
}

void copiar_string(char *origen, char *destino)
{
	int largo_origen = strlen(origen);
	int largo_dest = strlen(destino);

	memcpy(destino, origen, largo_origen);
}

t_registros* inicializar_registros()
{
	t_registros *registros = malloc(sizeof(t_registros));

	registros->ip = 0;
	memcpy(registros->ax, "0000", (sizeof(char) * 5));
	memcpy(registros->bx, "0000", (sizeof(char) * 5));
	memcpy(registros->cx, "0000", (sizeof(char) * 5));
	memcpy(registros->dx, "0000", (sizeof(char) * 5));
	memcpy(registros->eax, "00000000", (sizeof(char) * 9));
	memcpy(registros->ebx, "00000000", (sizeof(char) * 9));
	memcpy(registros->ecx, "00000000", (sizeof(char) * 9));
	memcpy(registros->edx, "00000000", (sizeof(char) * 9));
	memcpy(registros->rax, "0000000000000000", (sizeof(char) * 17));
	memcpy(registros->rbx, "0000000000000000", (sizeof(char) * 17));
	memcpy(registros->rcx, "0000000000000000", (sizeof(char) * 17));
	memcpy(registros->rdx, "0000000000000000", (sizeof(char) * 17));

	return registros;
}

void copiar_registros(t_registros *to_registros, t_registros *from_registros)
{
	to_registros->ip = from_registros->ip;
	copiar_string(from_registros->ax, to_registros->ax);
	copiar_string(from_registros->bx, to_registros->bx);
	copiar_string(from_registros->cx, to_registros->cx);
	copiar_string(from_registros->dx, to_registros->dx);
	copiar_string(from_registros->eax, to_registros->eax);
	copiar_string(from_registros->ebx, to_registros->ebx);
	copiar_string(from_registros->ecx, to_registros->ecx);
	copiar_string(from_registros->edx, to_registros->edx);
	copiar_string(from_registros->rax, to_registros->rax);
	copiar_string(from_registros->rbx, to_registros->rbx);
	copiar_string(from_registros->rcx, to_registros->rcx);
	copiar_string(from_registros->rdx, to_registros->rdx);
}

t_list* copiar_lista_instrucciones(t_list *nueva_lista, t_list *lista_instrucciones)
{
	int lineas = list_size(lista_instrucciones);

	for (int i = 0; i < lineas; i++)
	{
		t_instruc *instrucciones = list_get(lista_instrucciones, i);
		t_instruc *nueva_instruccion = malloc(sizeof(t_instruc));

		memcpy(&nueva_instruccion->nro, &instrucciones->nro, sizeof(uint32_t));

		memcpy(&nueva_instruccion->instruct_length, &instrucciones->instruct_length, sizeof(uint32_t));

		nueva_instruccion->instruct = malloc(instrucciones->instruct_length);
		memcpy(nueva_instruccion->instruct, instrucciones->instruct, instrucciones->instruct_length);

		memcpy(&nueva_instruccion->param1_length, &instrucciones->param1_length, sizeof(uint32_t));

		nueva_instruccion->param1 = malloc(instrucciones->param1_length);
		memcpy(nueva_instruccion->param1, instrucciones->param1, instrucciones->param1_length);

		memcpy(&nueva_instruccion->param2_length, &instrucciones->param2_length, sizeof(uint32_t));

		nueva_instruccion->param2 = malloc(instrucciones->param2_length);
		memcpy(nueva_instruccion->param2, instrucciones->param2, instrucciones->param2_length);

		memcpy(&nueva_instruccion->param3_length, &instrucciones->param3_length, sizeof(uint32_t));

		nueva_instruccion->param3 = malloc(instrucciones->param3_length);
		memcpy(nueva_instruccion->param3, instrucciones->param3, instrucciones->param3_length);

		list_add(nueva_lista, nueva_instruccion);
	}

	return nueva_lista;
}

void copiar_tabla_segmentos(tabla_segmentos_t* tabla_contexto,tabla_segmentos_t* tabla_pcb){
	uint32_t size = list_size(tabla_pcb->segmentos);
	tabla_contexto->pid = tabla_pcb->pid;

	for(int i = 0; i<size; i++){
		segmento_t* segmento = list_get(tabla_pcb->segmentos,i);
		segmento_t* nuevo_segmento = malloc(sizeof(segmento_t));

		nuevo_segmento->direccion_base = segmento->direccion_base;
		nuevo_segmento->ids = segmento->ids;
		nuevo_segmento->tamanio = segmento->tamanio;

		list_add(tabla_contexto->segmentos,nuevo_segmento);
	}
}

t_contexto* inicializar_contexto()
{
	t_contexto *contexto = malloc(sizeof(t_contexto));
	contexto->instrucciones = list_create();
	contexto->registros = inicializar_registros();
	contexto->param1 = malloc(sizeof(char) * 2);
	memcpy(contexto->param1, "0", (sizeof(char) * 2));
	contexto->param1_length = sizeof(char) * 2;
	contexto->param2 = malloc(sizeof(char) * 2);
	memcpy(contexto->param2, "0", (sizeof(char) * 2));
	contexto->param2_length = sizeof(char) * 2;
	contexto->param3 = malloc(sizeof(char) * 2);
	memcpy(contexto->param3, "0", (sizeof(char) * 2));
	contexto->param3_length = sizeof(char) * 2;
	contexto->estado = EXIT;
	contexto->pid = 0;

	contexto->tabla_segmento = malloc(sizeof(tabla_segmentos_t));
	contexto->tabla_segmento->pid = 0;
	contexto->tabla_segmento->segmentos = list_create();

	return contexto;
}

t_instruc_mem* inicializar_instruc_mem()
{
	t_instruc_mem* instruccion = malloc(sizeof(t_instruc_mem));
	instruccion->pid=0;
	instruccion->param1 = malloc(sizeof(char) * 2);
	memcpy(instruccion->param1, "0", (sizeof(char) * 2));
	instruccion->param1_length = sizeof(char) * 2;
	instruccion->param2 = malloc(sizeof(char) * 2);
	memcpy(instruccion->param2, "0", (sizeof(char) * 2));
	instruccion->param2_length = sizeof(char) * 2;
	instruccion->param3 = malloc(sizeof(char) * 2);
	memcpy(instruccion->param3, "0", (sizeof(char) * 2));
	instruccion->param3_length = sizeof(char) * 2;
	instruccion->estado = CREATE_SEGMENT;

	return instruccion;
}

void copiar_instruccion_mem(t_instruc_mem* instruccion, t_contexto* contexto){

	instruccion->param1_length = contexto->param1_length;
	instruccion->param2_length = contexto->param2_length;
	instruccion->param3_length = contexto->param3_length;
	instruccion->estado = contexto->estado;
	instruccion->pid = contexto->pid;

	instruccion->param1 = realloc(instruccion->param1,instruccion->param1_length);
	instruccion->param2 = realloc(instruccion->param2,instruccion->param2_length);
	instruccion->param3 = realloc(instruccion->param3,instruccion->param3_length);

	memcpy(instruccion->param1,contexto->param1,instruccion->param1_length);
	memcpy(instruccion->param2,contexto->param2,instruccion->param2_length);
	memcpy(instruccion->param3,contexto->param3,instruccion->param3_length);

}

void imprimir_segmentos(segmento_t* segmento){
	log_info(logger,"Segmento %d, base %d, tamanio %d",segmento->ids,segmento->direccion_base,segmento->tamanio);
}

void eliminar_segmentos(segmento_t* segmento , pcb_t* proceso){
	t_contexto* contexto_eliminar = inicializar_contexto();

	char* param1 = string_itoa(segmento->ids);

	uint32_t param1_length = string_length(param1) + 1;
	contexto_eliminar->param1 = realloc(contexto_eliminar->param1,param1_length);
	memcpy(contexto_eliminar->param1,param1,param1_length);
	contexto_eliminar->param1_length = param1_length;
	//En cuanto este arreglado descomentar
	contexto_eliminar->estado = DELETE_TABLE;
	//contexto_eliminar->estado = DELETE_SEGMENT;
	contexto_eliminar->pid = proceso->pid;
	//La peticion de eliminacion se hace directamente a memoria
	delete_segment(contexto_eliminar,proceso);
	log_info(logger,"Segmento eliminado %d", segmento->ids);
	free(contexto_eliminar->param1);
	free(contexto_eliminar);
}
void destroy_proceso(pcb_t *proceso)
{
	list_iterate(proceso->tabla_segmento->segmentos,(void*) imprimir_segmentos);
	//TODO: Cambiar logica para que se solicite la eliminacion de la tabla de segmentos completa
	while(list_size(proceso->tabla_segmento->segmentos) > 0){
	segmento_t* segmento = list_remove(proceso->tabla_segmento->segmentos,0);
	if(segmento->ids != 0){
		eliminar_segmentos(segmento,proceso);
		free(segmento);
	}
		}
	list_iterate(proceso->tabla_segmento->segmentos,(void*) imprimir_segmentos);
	solicitar_tabla_segmentos();
	free(proceso->tabla_segmento->segmentos);
	free(proceso->tabla_segmento);
	list_destroy_and_destroy_elements(proceso->instrucciones, (void*) instrucciones_destroy);
	free(proceso->recurso_bloqueante);
	devolver_instancias(proceso, lista_recursos);
	free(proceso->recursos_asignados);
	free(proceso->registros_cpu);
	free(proceso);
}

tabla_segmentos_t* buscar_tabla_segmentos(t_list* lista_tablas, int pid){
	bool buscar_tabla(tabla_segmentos_t* tabla){
		return tabla->pid == pid;
	}

	tabla_segmentos_t* tabla_encontrada = list_find(lista_tablas,buscar_tabla);

	return tabla_encontrada;
}

bool existe_tabla_segmentos(t_list* lista_tablas, int pid){
	bool encontrado = false;
	int size = list_size(lista_tablas);

	for(int i=0; i<size; i++){
		tabla_segmentos_t* tabla = list_get(lista_tablas,i);

		if((encontrado = tabla->pid == pid)) break;
	}

	return encontrado;
}

tabla_segmentos_t* solicitar_segmento_0(int pid){
	t_instruc_mem* instruccion = inicializar_instruc_mem();
	instruccion->estado = ALLOCATE_SEGMENT;
	instruccion->pid = pid;

	serializar_instruccion_memoria(memoria_connection, instruccion);

	solicitar_tabla_segmentos();

	tabla_segmentos_t* tabla = buscar_tabla_segmentos(lista_tabla_segmentos->lista,pid);

	return tabla;
}
