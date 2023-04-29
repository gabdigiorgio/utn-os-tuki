#ifndef TAD_H_
#define TAD_H_

#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <commons/collections/list.h>

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

//10 instrucciones
//ip vuelven en 5, entonces va a BLOCK
//ip vuelve en 10, va a EXIT

typedef struct
{
	uint16_t ip;
	char ax[5];
	char bx[5];
	char cx[5];
	char dx[5];
	char eax[9];
	char ebx[9];
	char ecx[9];
	char edx[9];
	char rax[17];
	char rbx[17];
	char rcx[17];
	char rdx[17];
} t_registros;

typedef struct
{
	t_registros* registros;
	t_list* instrucciones;
	//uint32_t pid;
} t_contexto;

#endif /* TAD_H_ */
