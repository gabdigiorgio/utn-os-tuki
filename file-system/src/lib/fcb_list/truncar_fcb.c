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
	t_list *lista_total_de_bloques = obtener_lista_total_de_bloques(id_fcb);
	int indice_inicial;
	int bloques_indirectos_agregados = 0;

	uint32_t offset_indirecto = valor_fcb(id_fcb,PUNTERO_INDIRECTO) * tamanio_de_bloque;

	for (int i = 0; i < cant_bloques_indirectos; i++)
	{
		offset_fcb_t *bloque = malloc(sizeof(offset_fcb_t));
		bloque->id_bloque = obtener_primer_bloque_libre();
		log_info(logger, "Bloque indirecto: %d", bloque->id_bloque);
		setear_bit_en_bitmap(bloque->id_bloque);
		list_add(lista_total_de_bloques, bloque);
		bloques_indirectos_agregados++;
	}

	indice_inicial = lista_total_de_bloques->elements_count - bloques_indirectos_agregados;

	escribir_bloques_indirectos(lista_total_de_bloques, indice_inicial, offset_indirecto);
}

void asignar_bloques(int id_fcb, int nuevo_tamanio)
{
	t_list *lista_total_de_bloques = obtener_lista_total_de_bloques(id_fcb);
	int size_inicial = list_size(lista_total_de_bloques);
	int cant_bloques_a_asignar = ceil((double) nuevo_tamanio / tamanio_de_bloque);
	int cant_bloques_actual = ceil((double) valor_fcb(id_fcb,TAMANIO_ARCHIVO) / tamanio_de_bloque);

	log_info(logger, "Ejecutando asignar bloques");

	for(int i = cant_bloques_actual; i<cant_bloques_a_asignar; i++){
		int id_bloque = obtener_primer_bloque_libre();
		offset_fcb_t *bloque = malloc(sizeof(offset_fcb_t));

		if (i == 0){
			size_inicial++;
			modificar_fcb(id_fcb, PUNTERO_DIRECTO, id_bloque);
			setear_bit_en_bitmap(id_bloque);
			bloque->id_bloque = id_bloque;
			list_add(lista_total_de_bloques, bloque);
			log_info(logger,"Bloque Directo %d", id_bloque);
			continue;
		}

		if (i == 1){
			size_inicial++;
			cant_bloques_a_asignar++;
			modificar_fcb(id_fcb, PUNTERO_INDIRECTO, id_bloque);
			setear_bit_en_bitmap(id_bloque);
			bloque->id_bloque = id_bloque;
			list_add(lista_total_de_bloques, bloque);
			log_info(logger,"Bloque Indirecto %d", id_bloque);
			continue;
		}

		bloque->id_bloque = id_bloque;
		setear_bit_en_bitmap(id_bloque);
		list_add(lista_total_de_bloques, bloque);
		log_info(logger,"Bloque Datos %d", id_bloque);
	}

	int size_final = list_size(lista_total_de_bloques);

	if(size_final > 2){
		uint32_t offset_indirecto = valor_fcb(id_fcb,PUNTERO_INDIRECTO) * tamanio_de_bloque;

		escribir_bloques_indirectos(lista_total_de_bloques, size_inicial, offset_indirecto);
	}

	log_info(logger, "Asigne: %d", cant_bloques_a_asignar);

	modificar_fcb(id_fcb, TAMANIO_ARCHIVO, nuevo_tamanio);
}

void desasignar_bloques(int id_fcb, int nuevo_tamanio)
{
	t_list *lista_de_bloques = obtener_lista_total_de_bloques(id_fcb);
	int tamanio_archivo = valor_fcb(id_fcb, TAMANIO_ARCHIVO);
	int cant_bloques_a_desasignar = ceil(((double)(tamanio_archivo - nuevo_tamanio) / tamanio_de_bloque)); // Usar ceil()

	int i = 0;
	int puntero_indirecto = 0;
	if(nuevo_tamanio <= tamanio_de_bloque && tamanio_archivo > tamanio_de_bloque){
		cant_bloques_a_desasignar++;
		puntero_indirecto = valor_fcb(id_fcb,PUNTERO_INDIRECTO);
	}

	while (i < cant_bloques_a_desasignar)
	{
		offset_fcb_t *bloque = list_pop(lista_de_bloques);
		if(bloque->id_bloque == puntero_indirecto) modificar_fcb(id_fcb,PUNTERO_INDIRECTO,0);
		log_info(logger, "Limpie: %d", bloque->id_bloque);
		limpiar_bit_en_bitmap(bloque->id_bloque);
		i++;
	}

	modificar_fcb(id_fcb, TAMANIO_ARCHIVO, nuevo_tamanio);
}
