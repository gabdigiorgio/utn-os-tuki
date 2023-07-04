#ifndef TAD_H_
#define TAD_H_

#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <pthread.h>
#include <commons/collections/list.h>
#include <commons/temporal.h>

typedef struct
{
	uint32_t nro;
	uint32_t instruct_length;
	char *instruct;
	uint32_t param1_length;
	char *param1;
	uint32_t param2_length;
	char *param2;
	uint32_t param3_length;
	char *param3;
} t_instruc;

typedef struct
{
	uint32_t size;
	void *stream;
} t_buffer;

typedef struct
{
	uint32_t codigo_operacion;
	uint32_t lineas;
	t_buffer *buffer;
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

typedef enum
{
	ERROR,
	SUCCESS_CREATE_SEGMENT,
	OUT_OF_MEMORY,
	COMPACTION_NEEDED,
	COMPACTATION_SUCCESS
}t_resp_mem;

typedef enum
{
	F_ERROR,
	F_OPEN_SUCCESS,
	F_CLOSE_SUCCESS,
	F_TRUNCATE_SUCCESS,
	F_WRITE_SUCCESS,
	F_SEEK_SUCCESS,
	F_READ_SUCCESS,
	F_CREATE_SUCCESS,
	F_DELETE_SUCCESS,
	FILE_DOESNT_EXISTS,
}t_resp_file;

typedef enum
{
	EXEC,
	EXIT,
	YIELD,
	IO,
	WAIT,
	SIGNAL,
	CREATE_SEGMENT,
	DELETE_SEGMENT,
	DELETE_TABLE,
	ALLOCATE_SEGMENT,
	PRINT_SEGMENTS,
	PRINT_MEMORY_DATA,
	MOV_IN,
	MOV_OUT,
	F_OPEN,
	F_CLOSE,
	F_SEEK,
	F_READ,
	F_WRITE,
	F_TRUNCATE,
	F_DELETE,
	F_CREATE,
	PRINT_FILE_STATE,
	PRINT_FILE_DATA
} contexto_estado_t;

typedef struct
{
	uint32_t ids;
	uint32_t direccion_base;
	uint32_t tamanio;
} segmento_t;

typedef struct
{
	uint32_t pid;
	t_list *segmentos;
} tabla_segmentos_t;

typedef struct
{
	t_registros *registros;
	t_list *instrucciones;
	uint32_t pid;
	char *param1;
	uint32_t param1_length;
	char *param2;
	uint32_t param2_length;
	char *param3;
	uint32_t param3_length;
	contexto_estado_t estado;
	tabla_segmentos_t *tabla_segmento;

} t_contexto;

typedef struct
{
	contexto_estado_t estado;
	uint32_t pid;
	uint32_t param1_length;
	char *param1;
	uint32_t param2_length;
	char *param2;
	uint32_t param3_length;
	char *param3;
} t_instruc_mem;

typedef struct
{
	contexto_estado_t estado;
	uint32_t pid;
	uint32_t param1_length;
	char *param1;
	uint32_t param2_length;
	char *param2;
	uint32_t param3_length;
	void *param3;
} t_instruc_mov;

typedef struct
{
	contexto_estado_t estado;
	uint32_t pid;
	uint32_t param1_length;
	char *param1;
	uint32_t param2_length;
	char *param2;
	uint32_t param3_length;
	char *param3;
	uint32_t param4_length;
	char *param4;
} t_instruc_file;

typedef struct
{
	uint32_t direccion_base;
	uint32_t tamanio;
} hueco_libre_t;

typedef struct
{
	char *instrucciones;
} lista_instucciones_t;

typedef struct
{
	uint32_t pid;
	char *nombre_archivo;
	char * posicion_puntero;
} archivo_abierto_t;

typedef enum
{
	PCB_NEW, PCB_READY, PCB_EXEC, PCB_BLOCK, PCB_EXIT
} pcb_estado_t;

typedef struct
{
	t_list *lista;
	pthread_mutex_t mutex;
} t_lista_mutex;

typedef struct
{
	int id; //corresponde con el indice
	char *nombre_recurso;
	int instancias;
	t_lista_mutex *cola_bloqueados;
	pthread_mutex_t mutex_instancias;
} t_recurso;

typedef struct
{
	uint32_t pid;
	uint32_t consola;
	char *recurso_bloqueante; //se podria utilizar un int y hacer referencia al id de la lista de recursos
	pcb_estado_t estado; 					// tipo de estado, puede ser un enum
	uint32_t estimado_proxima_rafaga; 		// se saca inicialmente del config
	t_temporal *tiempo_espera_en_ready; 	// se hace con timer, ver timestamp
	t_registros *registros_cpu; 			// crear struct de registros de cpu
	tabla_segmentos_t *tabla_segmento; 			// nada
	t_list *instrucciones; 						// lista recibida de consola
	t_list *tabla_archivos_abiertos; 	// nada
	t_list *recursos_asignados;
} pcb_t;

typedef struct
{
	uint32_t id;
	char* nombre_archivo;
	char* ruta_archivo;
	uint32_t tamanio_archivo;
	uint32_t puntero_directo;
	uint32_t puntero_indirecto;
	uint32_t puntero_archivo;
} fcb_t;

typedef struct
{
	t_list* lista_fcb;
} fcb_list_t;

typedef enum
{
	TAMANIO_ARCHIVO,
	PUNTERO_INDIRECTO,
	PUNTERO_DIRECTO,
	PUNTERO_ARCHIVO
} fcb_prop_t;

typedef struct
{
	uint32_t id_bloque;
	uint32_t offset;
} offset_fcb_t;

//modify(id, TAMANIO, 50)
//get(id)
//push(fcb_t)
//remove(id)
//find("Archivo1") = id
//inicializar() se fija todos los archivos .dat que estan cargados en la carpeta FCB, genera el fcb_t para cada uno y lo mete adentro de
//fcb_list_t

#endif /* TAD_H_ */
