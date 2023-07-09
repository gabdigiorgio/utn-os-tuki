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

						log_info(logger,"PID: %d - Accion: LEER - Direccion Fisica: %d - Tamanio: %d - Origen: FS",nueva_instruccion->pid,direccion_fisica,tamanio);

						direccion_fisica = atoi(nueva_instruccion->param1);
						tamanio = atoi(nueva_instruccion->param2);

						nueva_instruccion->param3 = realloc(nueva_instruccion->param3, tamanio);
						memcpy(nueva_instruccion->param3 , memoria + direccion_fisica , tamanio);
						nueva_instruccion->param3_length = tamanio;

						log_info(logger, "Lei el valor %s", nueva_instruccion->param3);

						serializar_instruccion_mov(server_connection, nueva_instruccion);

						break;

					case F_READ:

						log_info(logger,"PID: %d - Accion: ESCRIBIR - Direccion Fisica: %d - Tamanio: %d - Origen: FS",nueva_instruccion->pid,direccion_fisica,tamanio);

						direccion_fisica = atoi(nueva_instruccion->param1);
						tamanio = atoi(nueva_instruccion->param2);

						log_info(logger, "ESCRIBI el valor %s", (char*) nueva_instruccion->param3);

						memcpy(memoria + direccion_fisica, nueva_instruccion->param3 , tamanio);



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
