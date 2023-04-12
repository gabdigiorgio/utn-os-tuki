#ifndef TAD_H_
#define TAD_H_

#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <commons/collections/queue.h>

typedef struct
{
	uint32_t nro;
	uint32_t instruct_length;
	char* instruct;
	uint32_t param1_length;
    char* param1;
    uint32_t param2_length;
    char* param2;
    uint32_t param3_length;
    char* param3;
} t_instruc;

typedef enum
{
	INSTRUCCIONES,
}op_code;

typedef struct
{
	uint32_t size;
	void* stream;
} t_buffer;

typedef struct
{
	uint32_t codigo_operacion;
	uint32_t lineas;
	t_buffer* buffer;
} t_paquete;

#endif
