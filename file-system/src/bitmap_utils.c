#include "../includes/bitmap_utils.h"

void setear_bit_en_bitmap(int posicion)
{
	bitarray_set_bit(bitmap, posicion);
}

uint32_t obtener_bit_en_bitmap(int posicion)
{
	return bitarray_test_bit(bitmap, posicion);
}

uint32_t obtener_primer_bloque_libre()
{
	for (int i = 0; i < bitarray_get_max_bit(bitmap); i++)
	{
		if (bitarray_test_bit(bitmap, i) == 0)
		{
			return i;
		}
	}
	return -1;
}
