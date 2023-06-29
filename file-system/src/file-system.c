#include "../includes/file-system.h"

int main(int argc, char *argv[]) {
	logger = iniciar_logger();
	inicializar_fcb_list();

	/*int fcb_id = buscar_fcb("ParcialLeo");
	modificar_fcb(fcb_id,TAMANIO_ARCHIVO,540);
	log_info(logger,"%d",valor_fcb(fcb_id,TAMANIO_ARCHIVO));
	modificar_fcb(fcb_id,TAMANIO_ARCHIVO,250);
	log_info(logger,"%d",valor_fcb(fcb_id,TAMANIO_ARCHIVO));

	crear_fcb("ExamenGaby");
	int examen_gaby = buscar_fcb("ExamenGaby");
	modificar_fcb(examen_gaby,TAMANIO_ARCHIVO,700);
	log_info(logger,"%d",valor_fcb(examen_gaby,TAMANIO_ARCHIVO));
	borrar_fcb(examen_gaby);

	log_info(logger,"Ya lei");*/

	void* bloques = malloc(65532 * 64); //cantidad de bloques * tamaño de bloque
	int offset = 0;
	int size = 64;

	int fd;
	fd = open("./bloques.dat", O_RDWR);
	ftruncate(fd,65532 * 64);

	char* ptr = mmap(NULL,65532 * 64, PROT_READ | PROT_WRITE, MAP_SHARED, fd, 0);

	void* ejemplo = "DE ACA TENGO QUE EXTRAER EL NUMERO 5 DE TODO ESTE TEXTO VAMOS A VER COMO HAGO";
	int len = strlen(ejemplo);

	memcpy(ptr,ejemplo,len);

	char* numero = malloc(2);

	memcpy(numero,ptr + 35,1);

	log_info(logger,numero);

	void* algo = malloc(158);

	offset = 5; //index del bitarray, bloque 5

	//Genero una lista vacia de FCB
	fcb_list = list_create();

	//fcb puntero directo = 5, puntero indirecto = 42
	// 42 = 3, 50, 6540
	// 4 elementos -> void* memoria = malloc(4 * tam_bloque)
	// memcpy(memoria + offset,array_bloques + (id_bloque + tam_bloque), tam_bloque)
	// offset += tam_bloque
	//4 * 64
	//"HOLA/0"
	//"CHAU/0" -> 8 bytes
	//FSEEK 72 -> 64 + 8

	//te llega un F_TRUNCATE con tamaño 158 para el archivo 'archivo1'
	//osea, vos sabes que 158 / tamaño de bloque -> cantidad de bloques que necesitas
	//ej 158 / 64 -> 3 bloques ( aproximas para arriba )
	//agarrar la funcion del bitmap -> buscar los 3 primeros bloques libres, extraer id.
	//pusheas los 3 bloques a la lista de bloques (lista_bloques), por ej, bloque 1 2 y 3
	//FREAD 'archivo1'
	//tenes que copiar todos los datos de archivo1 a un espacio de memoria
	//sabes que tenes 3 bloques (list_size(lista_bloques))
	//tenes que hacer un malloc para el espacio entero de memoria, void* espacio = malloc(3 * tamaño de bloque)
	//recorres cada bloque de la lista
	//memcpy(espacio + offset_espacio, array_bloques + (id de bloque + tamaño de bloque), tamaño de bloque)
	//el bloque en si es un int, solo guarda la posicion que es igual a id.

	memcpy(bloques + (offset * size),&algo,64); // 5 * 64

	int resultado = 0;

	memcpy(&resultado,bloques + (offset * size),sizeof(int));

	log_info(logger,"resultado %d", resultado);

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
	array_de_bloques = malloc(sizeof(uint32_t)*cantidad_de_bloques); //cantidad de bloques

	tam_memoria_file_system=cantidad_de_bloques*tamanio_de_bloque;

	memoria_file_system = malloc(tam_memoria_file_system);

	inicializar_datos_memoria();

	memcpy(memoria_file_system,"ABCD",sizeof(char) * 5);

	log_info(logger,"%d",tam_memoria_file_system);

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

void asignar_bloques(int id_fcb, int cant_bloques)
{

	uint32_t bloque_directo = obtener_primer_bloque_libre();
	modificar_fcb(id_fcb, PUNTERO_DIRECTO, bloque_directo);
	setear_bit_en_bitmap(bloque_directo);

	log_info(logger, "Bloque directo: %d", bloque_directo);

	if (cant_bloques > 1)
	{
		uint32_t bloque_indirecto = obtener_primer_bloque_libre();
		modificar_fcb(id_fcb, PUNTERO_INDIRECTO, bloque_indirecto);
		setear_bit_en_bitmap(bloque_indirecto);

		log_info(logger, "Bloque indirecto: %d", bloque_indirecto);

		uint32_t puntero_indirecto = valor_fcb(id_fcb, PUNTERO_INDIRECTO);

		int offset = puntero_indirecto;
		for(int i = 0; i < cant_bloques - 1; i++)
		{
			uint32_t bloque = obtener_primer_bloque_libre();
			setear_bit_en_bitmap(bloque);

			log_info(logger, "Bloque apuntado: %d", bloque);
			memcpy(array_de_bloques + offset, &bloque, sizeof(uint32_t));
			offset += sizeof(uint32_t);
		}
	}

}

void desasignar_bloque(int id_fcb)
{

}

/*void desasignar_bloques(int id_fcb, int cant_bloques)
{
	int cant_bloques_fcb = valor_fcb(id_fcb, TAMANIO_ARCHIVO) / tamanio_de_bloque;

	for(int i = 0; i < cant_bloques; i++)
	{
		if(cant_bloques_fcb == 1)
		{
			limpiar_bit_en_bitmap(valor_fcb(id_fcb, PUNTERO_DIRECTO));
			return;
		}
		else
		{
			limpiar_bit_en_bitmap(valor_fcb(id_fcb, PUNTERO_DIRECTO));
			limpiar_bit_en_bitmap(valor_fcb(id_fcb, PUNTERO_INDIRECTO));

			uint32_t *array_de_bloques_indirectos = obtener_bloques_indirectos();

			for (int i = 0; i < cantidad_de_bloques_indirectos(); i++)
			{
				limpiar_bit_en_bitmap(array_de_bloques_indirectos[i]);
			}
		}

	}
}*/

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


