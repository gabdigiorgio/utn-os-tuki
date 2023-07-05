#include "../../includes/comm_threadFile.h"

void conexion_file_system(int server_connection){
	cpu_connection = esperar_cliente(server_connection);
	log_info(logger,"Se conecto FILE SYSTEM");

	while (exit_status == 0){
		t_paquete *paquete = malloc(sizeof(t_paquete));
		paquete->buffer = malloc(sizeof(t_buffer));
		deserializar_header(paquete, server_connection);

		switch (paquete->codigo_operacion){
			case 1:
				t_instruc_mov *nueva_instruccion = inicializar_instruc_mov();
				deserializar_instruccion_mov(nueva_instruccion, paquete->buffer, paquete->lineas);


				int direccion_fisica = 0;
				int tamanio = 0;

				switch (nueva_instruccion->estado){

					case F_WRITE:
						log_info(logger,"El proceso PID: %d solicito un F_WRITE",nueva_instruccion->pid);

						direccion_fisica = atoi(nueva_instruccion->param1);
						tamanio = atoi(nueva_instruccion->param2);

						nueva_instruccion->param3 = realloc(nueva_instruccion->param3, tamanio);

						memcpy(nueva_instruccion->param3 , memoria + direccion_fisica , tamanio);
						nueva_instruccion->param3_length = tamanio;


						log_info(logger, "Lei el valor %s", nueva_instruccion->param3);

						serializar_instruccion_mov(server_connection, nueva_instruccion);

						break;

					case F_READ:

						direccion_fisica = atoi(nueva_instruccion->param1);
						tamanio = atoi(nueva_instruccion->param2);

						memcpy(memoria + direccion_fisica, nueva_instruccion->param3 , tamanio);

						log_info(logger,"El proceso PID: %d solicito un F_READ",nueva_instruccion->pid);

						break;

					default:
						break;
				}
				break;
			default:
				exit_status = 1;
				break;
		}
		free(paquete->buffer->stream);
		free(paquete->buffer);
		free(paquete);
	}
}
