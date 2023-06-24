#include "../includes/file-system.h"

int main(int argc, char *argv[]) {
	logger = iniciar_logger();

	// array_de_bloques = malloc(sizeof(uint32_t)*cantidad_de_bloques); //cantidad de bloques
	int offset = 0;
	int size = 64;

	void* algo = malloc(158);

	offset = 5; //index del bitarray, bloque 5

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

	//memcpy(array_de_bloques + (offset * size),&algo,64); // 5 * 64

	int resultado = 0;

	//memcpy(&resultado,array_de_bloques + (offset * size),sizeof(int));

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




	log_info(logger,"%d",tam_memoria_file_system);

	//Inicializamos conexion con memoria
	if((memoria_connection = crear_conexion(memoria_ip,memoria_port)) == 0 || handshake_cliente(memoria_connection,3,4) == -1) {
		terminar_programa();
		return EXIT_FAILURE;
	}
	//log_info(logger,"%d",tamanio_archivo);
	int server_connection = iniciar_servidor(server_port);
	//generar_fcb();
	log_info(logger, "File System listo para recibir al Kernel");
	//log_info(logger,"%d",tamanio_archivo);
	int connection_fd = esperar_cliente(server_connection);
	log_info(logger,handshake(connection_fd));
	pthread_t thread_kernel;
	pthread_create(&thread_kernel, NULL, (void*) comm_threadKernel, connection_fd);
	pthread_join(thread_kernel, NULL);
	t_list* lista;

	while(1){};

	return EXIT_SUCCESS;
}
void iterator(char* value) {
	log_info(logger,"%s", value);
}

void generar_fcb(){
	if (config_has_property(fcb, "NOMBRE_ARCHIVO")){
			nombre_archivo = config_get_string_value(fcb, "NOMBRE_ARCHIVO");
			log_info(logger, "%s", nombre_archivo);
		}
	if (config_has_property(fcb, "TAMANIO_ARCHIVO")){
			tamanio_archivo = config_get_int_value(fcb, "TAMANIO_ARCHIVO");
			log_info(logger, "%d", tamanio_archivo);
		}
	if (config_has_property(fcb, "PUNTERO_DIRECTO")){
			puntero_directo = config_get_int_value(fcb, "PUNTERO_DIRECTO");
			log_info(logger, "%d", puntero_directo);
		}
	if (config_has_property(fcb, "PUNTERO_INDIRECTO")){
			puntero_indirecto = config_get_int_value(fcb, "PUNTERO_INDIRECTO");
			log_info(logger, "%d", puntero_indirecto);
		}
	log_info(logger, "FCB Creado");
}

void terminar_programa()
{
	log_destroy(logger);
	config_destroy(config);
	config_destroy(superbloque_config);
	config_destroy(fcb);
	liberar_conexion(memoria_connection);
}

void asignar_bloque(fcb_t* archivo)
{
     archivo->puntero_directo=obtener_pirmer_bloque_libre();
     //que hago con el puntero indirecto?
     //actualizar bitmap
    // setear_bit_en_bitmap(archivo->puntero_directo);
}

void escribir_bloque(uint32_t bloque_a_escribir){
	 memcpy(memoria_file_system, &bloque_a_escribir, sizeof(uint32_t));
	 memoria_file_system += sizeof(uint32_t);

}
void escribir_bytes(int bytes_a_escribir)
{


	uint32_t cantidad_de_bloques_a_escribir = ceil(cantidad_de_bloques / bytes_a_escribir);

	//uint32_t primer_bloque_libre = obtener_primer_bloque_libre();
   //verificar de tener los bloques necesarios para escribir
	      for(uint32_t i =0; i < cantidad_de_bloques_a_escribir; i++)
	      {
	       escribir_bloque(array_de_bloques[i]);
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



