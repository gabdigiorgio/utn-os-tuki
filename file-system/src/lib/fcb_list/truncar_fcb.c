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

void asignar_bloques(int id_fcb, int nuevo_tamanio)
{
	int tamanio_archivo = valor_fcb(id_fcb, TAMANIO_ARCHIVO);
	int cant_bloques_fcb = (nuevo_tamanio - tamanio_archivo) / tamanio_de_bloque; // Usar ceil()
	modificar_fcb(id_fcb, TAMANIO_ARCHIVO, nuevo_tamanio);


	if (cant_bloques_fcb > 0)
	{
		//id_bloque * tamanio_de_bloque = offset absoluto
		//vos vas a saber cuantos bloques nuevos necesitas
		//te paras en el ultimo id_bloque y calculas el offset de ese + 4
		//luego iteras con la cantidad de bloques que necesitas
		//y escribis el bloque en memoria corriendo el offset 4 por cada uno
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
	t_list *lista_de_bloques = obtener_lista_total_de_bloques(id_fcb);
	int tamanio_archivo = valor_fcb(id_fcb, TAMANIO_ARCHIVO);
	int cant_bloques_a_desasignar = (tamanio_archivo - nuevo_tamanio) / tamanio_de_bloque; // Usar ceil()
	modificar_fcb(id_fcb, TAMANIO_ARCHIVO, nuevo_tamanio);

	int i = 0;

	while (i < cant_bloques_a_desasignar)
	{
		offset_fcb_t *bloque = list_pop(lista_de_bloques);
		log_info(logger, "Limpie: %d", bloque->id_bloque);
		limpiar_bit_en_bitmap(bloque->id_bloque);
		i++;
	}
}