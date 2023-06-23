#include "../includes/mmu.h"

int traducir_direccion(char *param, t_contexto *contexto)
{
	int direccion_fisica;
	int direccion_logica = atoi(param);

	int num_segmento = floor(direccion_logica / tam_max_segmento);
	int desplazamiento_segmento = direccion_logica % tam_max_segmento;
	int direccion_base_segmento = obtener_direccion_base(num_segmento, contexto->tabla_segmento);
	int tamanio_segmento = obtener_tamanio_segmento(num_segmento, contexto->tabla_segmento);

	if (direccion_base_segmento + desplazamiento_segmento > tamanio_segmento)
	{
		//devolver el contexto de ejecucion a kernel y matar proceso
		log_error(logger, "PID: %d - Error SEG_FAULT - Segmento: %d - Offset: %d - Tamanio: %d",
				contexto->pid, num_segmento, desplazamiento_segmento, tamanio_segmento);
		contexto_estado = EXIT;
		return 1;
	}

	direccion_fisica = direccion_base_segmento + desplazamiento_segmento;

	snprintf(param, sizeof(param), "%d", direccion_fisica);

	log_info(logger, "[MMU] - Segmento: %d - Direccion fisica: %d", num_segmento, direccion_fisica);

	return 0;
}

segmento_t* buscar_segmento(int num_segmento, t_list *segmentos)
{
	for (int i = 0; i < list_size(segmentos); i++)
	{
		segmento_t *segmento = (segmento_t*) list_get(segmentos, i);

		if (segmento->ids == num_segmento)
		{
			return segmento;
		}
	}

	return NULL;
}

int obtener_direccion_base(int num_segmento, tabla_segmentos_t *tabla_segmentos)
{
	segmento_t *segmento = buscar_segmento(num_segmento, tabla_segmentos->segmentos);

	if (segmento != NULL)
	{
		return segmento->direccion_base;
	}
	else
	{
		return -1;
	}
}

int obtener_tamanio_segmento(int num_segmento, tabla_segmentos_t *tabla_segmentos)
{
	segmento_t *segmento = buscar_segmento(num_segmento, tabla_segmentos->segmentos);

	if (segmento != NULL)
	{
		return segmento->tamanio;
	}
	else
	{
		return -1;
	}
}
