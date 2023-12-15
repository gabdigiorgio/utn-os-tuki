#include "../includes/memoria.h"

int main(int argc, char *argv[])
{

	logger = iniciar_logger();

	segmento_t* segmento_0 = malloc(sizeof(segmento_t));
	segmento_0->ids=0;
	segmento_0->direccion_base=0;
	segmento_0->tamanio = tam_segmento_0;

	//Inicializamos las variables globales desde el config, que loggee errores o success si todo esta bien

	if (argc < 2)
	{
		log_error(logger, "Falta parametro del path del archivo de configuracion");
		return EXIT_FAILURE;
	}

	config = iniciar_config(argv[1]);

	exit_status = initial_setup();

	if (exit_status == EXIT_FAILURE)
	{
		return EXIT_FAILURE;
	}

	// Asigno tamaño a la memoria, creo lista de tablas de segmentos, de huecos_libres y segmento 0

	memoria = malloc(tam_memoria);
	inicializar_datos_memoria();

	segmento_0->tamanio = tam_segmento_0;
	segmento_0->direccion_base = 0;

	lista_de_tablas = list_create();

	tabla_segmentos_t* tabla_segmento_0 = malloc(sizeof(tabla_segmentos_t));

	tabla_segmento_0->pid = 0;
	tabla_segmento_0->segmentos = list_create();
	list_add(tabla_segmento_0->segmentos, segmento_0);
	list_add(lista_de_tablas,tabla_segmento_0);

	lista_de_huecos_libres = list_create();

	hueco_libre_t *primer_hueco_libre = malloc(sizeof(hueco_libre_t));
	primer_hueco_libre->direccion_base = segmento_0->tamanio;
	primer_hueco_libre->tamanio = tam_memoria - segmento_0->tamanio;
	list_add(lista_de_huecos_libres,primer_hueco_libre);

	//mando un dato temporal

	char* ejemplo = "A";

	memcpy(memoria,ejemplo,2);

	// Conectamos al monitor, comentar para la entrega

	int monitor_connection = crear_conexion("127.0.0.1","8040");

	pthread_t thread_mon;
	pthread_create(&thread_mon, NULL, (void*) thread_monitor, monitor_connection);
	pthread_detach(thread_mon);

	// Esperamos conexiones de Kernel, CPU y File-System

	int server_connection = iniciar_servidor(server_port);

	cpu_conectada = false;
	kernel_conectado = false;
	fileSystem_conectado = false;
	log_info(logger, "Memoria lista para recibir al CPU, Kernel o File System");
	pthread_t threads[3];
	int num_threads = 0;

	while (exit_status == 0)
	{
		if(num_threads < CANTIDAD_DE_THREADS){
			t_conexion *conexion = malloc(sizeof *conexion);
			conexion->num_socket = esperar_cliente(server_connection);
			conexion->id_cliente = num_threads;

			pthread_create(&threads[num_threads], NULL, (void*) thread_main, conexion);
			pthread_detach(threads[num_threads]);
			num_threads++;
		}
	}

	log_info(logger, "Terminando modulo MEMORIA");
	liberar_conexion(server_connection);

}

void thread_main(t_conexion* conexion)
{
	log_info(logger, "Thread iniciado correctamente");
	char *mensaje = handshake(conexion->num_socket);
	log_info(logger,"%s", mensaje);

	if (strcmp(mensaje, "Handshake de CPU recibido correctamente") == 0)
	{
		conexion_cpu(conexion->num_socket);
	}
	else if (strcmp(mensaje, "Handshake de Kernel recibido correctamente") == 0)
	{
		conexion_kernel(conexion->num_socket);
	}
	else if (strcmp(mensaje, "Handshake de File System recibido correctamente") == 0)
	{
		conexion_file_system(conexion->num_socket);
	}

	free(conexion);
}

void create_tabla_segmento(int pid)
{
	tabla_segmentos_t *tabla = malloc(sizeof(tabla_segmentos_t));
	tabla->pid = pid;
	tabla->segmentos = list_create();
	log_info(logger, "%d", list_size(lista_de_tablas));
	log_info(logger, "%d", list_size(tabla->segmentos));
	list_add(lista_de_tablas, tabla);
	log_info(logger, "%d", list_size(lista_de_tablas));

}

void thread_monitor(int connection){
	while(1){
		usleep(250);
		serializar_memoria(connection,memoria,tam_memoria);
	}
}
