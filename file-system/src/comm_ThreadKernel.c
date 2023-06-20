#include "../includes/comm_ThreadKernel.h"

void comm_threadKernel(int kernel_connection){
	int exit_status = 0;
	while(exit_status == 0){
		t_paquete *paquete = malloc(sizeof(t_paquete));
		paquete->buffer = malloc(sizeof(t_buffer));
		deserializar_header(paquete, kernel_connection);

		switch (paquete->codigo_operacion){
			case 1:
				t_instruc_file *nueva_instruccion = inicializar_instruc_file();
				deserializar_instruccion_file(nueva_instruccion, paquete->buffer, paquete->lineas);
				uint32_t pid = nueva_instruccion->pid;
				t_resp_file estado_file = F_ERROR;
				switch (nueva_instruccion->estado){
					case F_OPEN:
						estado_file = F_OPEN_SUCCESS;
						log_info(logger,"PID: %d solicito F_OPEN para el archivo %s",pid, nueva_instruccion->param1);
						serializar_respuesta_file_kernel(kernel_connection, estado_file);
						break;
					case F_CREATE:
						estado_file = F_CREATE_SUCCESS;
						log_info(logger,"PID: %d solicito F_CREATE para el archivo %s",pid, nueva_instruccion->param1);
						serializar_respuesta_file_kernel(kernel_connection, estado_file);
						break;
					case F_CLOSE:
						estado_file = F_CLOSE_SUCCESS;
						log_info(logger,"PID: %d solicito F_CLOSE para el archivo %s",pid, nueva_instruccion->param1);
						serializar_respuesta_file_kernel(kernel_connection, estado_file);
						break;
					case F_TRUNCATE:
						estado_file = F_TRUNCATE_SUCCESS;
						log_info(logger,"PID: %d solicito F_TRUNCATE para el archivo %s",pid, nueva_instruccion->param1);
						serializar_respuesta_file_kernel(kernel_connection, estado_file);
						break;
					case F_WRITE:
						realizar_f_write(nueva_instruccion);
						estado_file = F_WRITE_SUCCESS;
						log_info(logger,"PID: %d solicito F_WRITE para el archivo %s",pid, nueva_instruccion->param1);
						serializar_respuesta_file_kernel(kernel_connection, estado_file);
						break;
					case F_READ:
						realizar_f_read(nueva_instruccion);
						estado_file = F_READ_SUCCESS;
						log_info(logger,"PID: %d solicito F_READ para el archivo %s",pid, nueva_instruccion->param1);
						serializar_respuesta_file_kernel(kernel_connection, estado_file);
						break;
					case F_SEEK:
						estado_file = F_SEEK_SUCCESS;
						log_info(logger,"PID: %d solicito F_SEEK para el archivo %s",pid, nueva_instruccion->param1);
						serializar_respuesta_file_kernel(kernel_connection, estado_file);
						break;
					default:
						break;
				}
				free(nueva_instruccion->param1);
				free(nueva_instruccion->param2);
				free(nueva_instruccion->param3);
				free(nueva_instruccion);
				break;
			default:
				break;
		}
		free(paquete->buffer->stream);
		free(paquete->buffer);
		free(paquete);
	}
}
