#include "../../../includes/fcb_list.h"

int truncar_fcb(char *nombre_fcb, int nuevo_tamanio)
{
	int resultado = -1;
	int id_fcb = buscar_fcb(nombre_fcb);

	if (id_fcb != -1)
	{
		int tamanio_archivo = valor_fcb(id_fcb, TAMANIO_ARCHIVO);

		if (nuevo_tamanio > tamanio_archivo)
		{
			asignar_bloques(id_fcb, nuevo_tamanio);
		}
		else if (nuevo_tamanio < tamanio_archivo)
		{
			desasignar_bloques(id_fcb, nuevo_tamanio);
		}

		resultado = 0;
	}

	return resultado;
}

void asignar_bloque_directo(int id_fcb)
{
	uint32_t bloque_directo = obtener_primer_bloque_libre();
	modificar_fcb(id_fcb, PUNTERO_DIRECTO, bloque_directo);
	setear_bit_en_bitmap(bloque_directo);
}

void asignar_bloque_indirecto(int id_fcb)
{
	uint32_t bloque_indirecto = obtener_primer_bloque_libre();
	modificar_fcb(id_fcb, PUNTERO_INDIRECTO, bloque_indirecto);
	setear_bit_en_bitmap(bloque_indirecto);
}

void asignar_bloques_indirectos(int id_fcb, int cant_bloques_indirectos)
{
	t_list *lista_de_bloques = obtener_lista_total_de_bloques(id_fcb);

	for (int i = 0; i < cant_bloques_indirectos; i++)
	{
		uint32_t bloque = obtener_primer_bloque_libre();
		setear_bit_en_bitmap(bloque);
		escribir_bloques_indirectos(lista_de_bloques, lista_de_bloques->elements_count);
	}
}

void asignar_bloques(int id_fcb, int nuevo_tamanio)
{
	int tamanio_archivo = valor_fcb(id_fcb, TAMANIO_ARCHIVO);
	int cant_bloques_a_asignar = ceil((double) ((nuevo_tamanio - tamanio_archivo) / tamanio_de_bloque));

	log_info(logger, "Ejecutando asignar bloques");

	if (tamanio_archivo == 0)
	{
		if (cant_bloques_a_asignar > 0)
		{
			asignar_bloque_directo(id_fcb);

			if (cant_bloques_a_asignar > 1)
			{
				asignar_bloque_indirecto(id_fcb);
				int cant_bloques_indirectos = cant_bloques_a_asignar - 1;
				asignar_bloques_indirectos(id_fcb, cant_bloques_indirectos);
			}
		}
	}
	else if (tamanio_archivo > 0 && tamanio_archivo <= tamanio_de_bloque)
	{
		if (cant_bloques_a_asignar > 0)
		{
			asignar_bloque_indirecto(id_fcb);
			int cant_bloques_indirectos = cant_bloques_a_asignar - 1;
			asignar_bloques_indirectos(id_fcb, cant_bloques_indirectos);
		}
	}
	else if (tamanio_archivo > tamanio_de_bloque)
	{
		if (cant_bloques_a_asignar > 0)
		{
			int cant_bloques_indirectos = cant_bloques_a_asignar - 1;
			asignar_bloques_indirectos(id_fcb, cant_bloques_indirectos);
		}
	}

	log_info(logger, "Asigne: %d", cant_bloques_a_asignar);

	modificar_fcb(id_fcb, TAMANIO_ARCHIVO, nuevo_tamanio);
}

void desasignar_bloques(int id_fcb, int nuevo_tamanio)
{
	t_list *lista_de_bloques = obtener_lista_total_de_bloques(id_fcb);
	int tamanio_archivo = valor_fcb(id_fcb, TAMANIO_ARCHIVO);
	int cant_bloques_a_desasignar = (tamanio_archivo - nuevo_tamanio) / tamanio_de_bloque; // Usar ceil()

	int i = 0;

	while (i < cant_bloques_a_desasignar)
	{
		offset_fcb_t *bloque = list_pop(lista_de_bloques);
		log_info(logger, "Limpie: %d", bloque->id_bloque);
		limpiar_bit_en_bitmap(bloque->id_bloque);
		i++;
	}

	modificar_fcb(id_fcb, TAMANIO_ARCHIVO, nuevo_tamanio);
}
