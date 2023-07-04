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

int obtener_cantidad_de_bloques(int id_fcb)
{
	int tamanio_fcb = valor_fcb(id_fcb, TAMANIO_ARCHIVO);
	int cant_bloques_fcb = tamanio_fcb / tamanio_de_bloque; // Usar ceil()

	return cant_bloques_fcb;
}

t_list* obtener_lista_de_bloques(int id_fcb)
{
	t_list *lista_de_bloques = list_create();
	int cant_bloques_fcb = obtener_cantidad_de_bloques(id_fcb);

	t_bloque *bloque_directo = malloc(sizeof(t_bloque));
	bloque_directo->id_bloque = valor_fcb(id_fcb, PUNTERO_DIRECTO);

	list_add(lista_de_bloques, bloque_directo);

	if (cant_bloques_fcb > 1)
	{
		uint32_t bloque_indirecto = valor_fcb(id_fcb, PUNTERO_INDIRECTO);
		int offset = 0;
		int cant_bloques_indirectos = cant_bloques_fcb - 2;

		for (int i = 0; i < cant_bloques_indirectos; i++)
		{
			t_bloque *bloque = malloc(sizeof(t_bloque));
			memcpy(&bloque->id_bloque, memoria_file_system + (bloque_indirecto * tamanio_de_bloque) + offset, sizeof(uint32_t));
			list_add(lista_de_bloques, bloque);
			offset += sizeof(uint32_t);
		}
	}

	return lista_de_bloques;
}

void asignar_bloques(int id_fcb, int nuevo_tamanio)
{
	int tamanio_archivo = valor_fcb(id_fcb, TAMANIO_ARCHIVO);
	int cant_bloques_fcb = (nuevo_tamanio - tamanio_archivo) / tamanio_de_bloque; // Usar ceil()
	modificar_fcb(id_fcb, TAMANIO_ARCHIVO, nuevo_tamanio);

	if (cant_bloques_fcb > 0)
	{
		uint32_t bloque_directo = obtener_primer_bloque_libre();
		modificar_fcb(id_fcb, PUNTERO_DIRECTO, bloque_directo);
		setear_bit_en_bitmap(bloque_directo);

		log_info(logger, "Bloque directo: %d", bloque_directo);

		if (cant_bloques_fcb > 1)
		{
			uint32_t bloque_indirecto = obtener_primer_bloque_libre();
			modificar_fcb(id_fcb, PUNTERO_INDIRECTO, bloque_indirecto);
			setear_bit_en_bitmap(bloque_indirecto);

			log_info(logger, "Bloque indirecto: %d", bloque_indirecto);

			uint32_t puntero_indirecto = valor_fcb(id_fcb, PUNTERO_INDIRECTO);
			int offset = 0;
			int cant_bloques_indirectos = cant_bloques_fcb - 2;

			for (int i = 0; i < cant_bloques_indirectos; i++)
			{
				uint32_t bloque = obtener_primer_bloque_libre();
				setear_bit_en_bitmap(bloque);

				char *string = string_itoa(bloque);
				void *string2 = malloc(4);
				memcpy(string2, "0000", 4);
				memcpy(string2 + (4 - strlen(string)), string, strlen(string));
				log_info(logger, "Bloque apuntado: %d", bloque);
				memcpy(memoria_file_system + (puntero_indirecto * tamanio_de_bloque) + offset, &bloque, sizeof(uint32_t));
				offset += sizeof(uint32_t);
			}
		}
	}

}

void desasignar_bloques(int id_fcb, int nuevo_tamanio)
{
	t_list *lista_de_bloques = obtener_lista_de_bloques(id_fcb);
	int tamanio_archivo = valor_fcb(id_fcb, TAMANIO_ARCHIVO);
	int cant_bloques_a_desasignar = (tamanio_archivo - nuevo_tamanio) / tamanio_de_bloque; // Usar ceil()
	modificar_fcb(id_fcb, TAMANIO_ARCHIVO, nuevo_tamanio);

	int i = 0;

	while (i < cant_bloques_a_desasignar)
	{
		t_bloque *bloque = list_pop(lista_de_bloques);
		log_info(logger, "Limpie: %d", bloque->id_bloque);
		limpiar_bit_en_bitmap(bloque->id_bloque);
		i++;
	}
}
