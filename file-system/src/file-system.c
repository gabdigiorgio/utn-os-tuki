#include "../includes/file-system.h"

int main(int argc, char *argv[]) {
	logger = iniciar_logger();
	inicializar_fcb_list();

	//Inicializamos las variables globales desde el config, que loggee errores o success si todo esta bien
	if (argc < 2) {
		 log_error(logger, "Falta parametro del path del archivo de configuracion");
		 return EXIT_FAILURE;
	 }
	config = iniciar_config(argv[1]);
	int exit_status = initial_setup();
	if (exit_status==EXIT_FAILURE){
		return EXIT_FAILURE;
	}

	superbloque_config = iniciar_config(path_superbloque);

	exit_status = leer_superbloque_config();
	if (exit_status==EXIT_FAILURE){
		return EXIT_FAILURE;
	}

	tam_memoria_file_system=cantidad_de_bloques*tamanio_de_bloque;

	//Mapeo de memoria
	int fd;
	fd = open("./bloques.dat", O_RDWR);
	ftruncate(fd,tam_memoria_file_system);

	memoria_file_system = mmap(NULL,tam_memoria_file_system, PROT_READ | PROT_WRITE, MAP_SHARED, fd, 0);

	inicializar_datos_memoria();

	/*
	memcpy(memoria_file_system,"0123456789ABCDEF",sizeof(char) * 16);
	int bloque1 = 2;
	int bloque2 = 3;
	int bloque3 = 4;
	memcpy(memoria_file_system + 16, &bloque1, sizeof(uint32_t));
	memcpy(memoria_file_system + 20, &bloque2, sizeof(uint32_t));
	memcpy(memoria_file_system + 24, &bloque3, sizeof(uint32_t));

	memcpy(memoria_file_system + 32,"FEDCBA9876543210",sizeof(char) * 16);
	memcpy(memoria_file_system + 48,"0123456789ABCDEF",sizeof(char) * 16);
	memcpy(memoria_file_system + 64,"FEDCBA9876543210",sizeof(char) * 16);
	*/

	exit_status = crear_bitmap();
	if (exit_status == EXIT_FAILURE)
	{
		return EXIT_FAILURE;
	}

	//Verificar si existe el directorio

	log_info(logger,"cant de bits del bitmap %d", bitarray_get_max_bit(bitmap));

	// Conectamos al monitor, comentar para la entrega

	int monitor_connection = crear_conexion("127.0.0.1","8050");

	pthread_t thread_mon;
	pthread_create(&thread_mon, NULL, (void*) thread_monitor, monitor_connection);
	pthread_detach(thread_mon);

	//Inicializamos conexion con memoria
	if((memoria_connection = crear_conexion(memoria_ip,memoria_port)) == 0 || handshake_cliente(memoria_connection,3,4) == -1) {
		terminar_programa();
		return EXIT_FAILURE;
	}
	//log_info(logger,"%d",tamanio_archivo);
	int server_connection = iniciar_servidor(server_port);

	log_info(logger, "File System listo para recibir al Kernel");
	//log_info(logger,"%d",tamanio_archivo);
	int connection_fd = esperar_cliente(server_connection);
	log_info(logger,"%s",handshake(connection_fd));
	pthread_t thread_kernel;
	pthread_create(&thread_kernel, NULL, (void*) comm_threadKernel, connection_fd);
	pthread_join(thread_kernel, NULL);

	return EXIT_SUCCESS;
}
void iterator(char* value) {
	log_info(logger,"%s", value);
}

void terminar_programa()
{
	log_destroy(logger);
	config_destroy(config);
	config_destroy(superbloque_config);
	config_destroy(fcb);
	liberar_conexion(memoria_connection);
}

/*
void escribir_bloque(uint32_t bloque_a_escribir, void* datos){
	int tamanio_datos = strlen(datos) + 1;
	memcpy(memoria_file_system + (bloque_a_escribir * tamanio_bloque), datos, tamanio_datos);
}

void escribir_bytes(int bytes_a_escribir, void* todos_los_datos)
{
	uint32_t cantidad_de_bloques_a_escribir = ceil(cantidad_de_bloques / bytes_a_escribir);

	//uint32_t primer_bloque_libre = obtener_primer_bloque_libre();
   //verificar de tener los bloques necesarios para escribir
	      for(uint32_t i =0; i < cantidad_de_bloques_a_escribir; i++)
	      {

	       escribir_bloque(i,todos_los_datos + (i * tamanio_bloque));
	      }

}

uint32_t leer_bloque(uint32_t id_bloque_a_leer)
{
      uint32_t bloque;
      memcpy(bloque, (char*)(memoria_file_system + id_bloque_a_leer * sizeof(uint32_t)), sizeof(uint32_t));
      //creo q actualizar offset?
      return bloque;
}
void leer_bytes(int bytes_a_leer)
{
       uint32_t cantidad_de_bloques_a_leer= ceil(cantidad_de_bloques / bytes_a_leer);
      for(uint32_t i = 0; i < cantidad_de_bloques_a_leer; i++)
      {
            leer_bloque(array_de_bloques[i]);
      }
}
*/

void thread_monitor(int connection){
	while(1){
		usleep(1000);
		serializar_memoria(connection,memoria_file_system,tam_memoria_file_system);
	}
}


