#ifndef BITMAP_UTILS_H_
#define BITMAP_UTILS_H_

#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <commons/bitarray.h>

// Global variables
extern t_bitarray *bitmap;

// Functions
uint32_t obtener_primer_bloque_libre();

#endif /* BITMAP_UTILS_H_ */
