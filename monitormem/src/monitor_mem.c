#include "../includes/monitor_mem.h"

int main(int argc, char *argv[]){

	logger = iniciar_logger();
	config = iniciar_config(argv[1]);

	int exit_status = initial_setup();

	if (exit_status == EXIT_FAILURE)
	{
		return EXIT_FAILURE;
	}

	int server_connection = iniciar_servidor(server_port);

	printf("Esperando memoria");
	fflush(stdout);

	conexion_memoria(server_connection);
}

void conexion_memoria(int server_connection){
	int exit_status = 0;
	while(1){
		int connection_fd = esperar_cliente(server_connection);
		printf("\r Conectado con memoria");
		exit_status = 0;

		while (exit_status == 0) {
			//Reservo memoria para el paquete
			t_paquete* paquete = malloc(sizeof(t_paquete));
			paquete->buffer = malloc(sizeof(t_buffer));

			//Recivo el header del paquete + el stream de datos
			deserializar_header(paquete, connection_fd);

			//Reviso el header para saber de que paquete se trata y deserealizo acorde
			switch(paquete->codigo_operacion){
				case 1:
					imprimir_datos(paquete->buffer->stream,paquete->buffer->size);
					break;
				default:
					exit_status = 1;
					break;
			}

			fflush(stdout);

			free(paquete->buffer->stream);
			free(paquete->buffer);
			free(paquete);
		}
	}
}
