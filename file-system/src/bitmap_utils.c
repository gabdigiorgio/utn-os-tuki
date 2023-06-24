#include "../includes/bitmap_utils.h"

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
