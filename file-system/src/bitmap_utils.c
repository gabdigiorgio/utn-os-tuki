#include "../includes/bitmap_utils.h"

void setear_bit_en_bitmap(uint32_t id_bloque) // Setea el bit en 1 para el bloque indicado
{
	bitarray_set_bit(bitmap, id_bloque);
}

void limpiar_bit_en_bitmap(uint32_t id_bloque) // Setea el bit en 0 para el bloque indicado
{
	bitarray_clean_bit(bitmap, id_bloque);
}

uint32_t obtener_bit_en_bitmap(uint32_t id_bloque) // Devuelve el bit del bloque indicado
{
	return bitarray_test_bit(bitmap, id_bloque);
}

uint32_t obtener_primer_bloque_libre()
{
	for (uint32_t i = 0; i < bitarray_get_max_bit(bitmap); i++)
	{
		if (bitarray_test_bit(bitmap, i) == 0)
		{
			return i;
		}
	}
	return -1;
}
