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

	int fd;
	fd = open("./bloques.dat", O_RDWR);
	ftruncate(fd,tam_memoria_file_system);

	memoria_file_system = mmap(NULL,tam_memoria_file_system, PROT_READ | PROT_WRITE, MAP_SHARED, fd, 0);

	inicializar_datos_memoria();

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

	int id = buscar_fcb("PRUEBA");
		log_info(logger,"Buscado FCB %d", id);

		void imprimir_bloques(offset_fcb_t* offset){
			log_info(logger,"Bloque ID: %d, Offset: %d",offset->id_bloque,offset->offset);
		}

		void imprimir_bloques2(offset_fcb_t* offset){
			log_info(logger,"Bloque ID: %d, Offset: %d, tamanio: %d",offset->id_bloque,offset->offset,offset->tamanio);
		}

		t_list* lista_bloques1 = obtener_lista_total_de_bloques(id);

		log_info(logger,"Lista 1");
		list_iterate(lista_bloques1,(void*) imprimir_bloques);

		t_list* lista_bloques2= armar_lista_offsets(id,36,20);

		log_info(logger,"Lista 2");
		list_iterate(lista_bloques2,(void*) imprimir_bloques2);

		t_list* lista_bloques3= armar_lista_offsets(id,20,14);

		log_info(logger,"Lista 3");
		list_iterate(lista_bloques3,(void*) imprimir_bloques2);

		t_list* lista_bloques4= armar_lista_offsets(id,18,0);

		log_info(logger,"Lista 4");
		list_iterate(lista_bloques4,(void*) imprimir_bloques2);

		t_list* lista_bloques5= armar_lista_offsets(id,46,18);

		log_info(logger,"Lista 5");
		list_iterate(lista_bloques5,(void*) imprimir_bloques2);

		t_list* lista_bloques6= armar_lista_offsets(id,64,0);

		log_info(logger,"Lista 6");
		list_iterate(lista_bloques6,(void*) imprimir_bloques2);

		t_list* lista_bloques7= armar_lista_offsets(id,4,20);

		log_info(logger,"Lista 7");
		list_iterate(lista_bloques7,(void*) imprimir_bloques2);

		t_list* lista_bloques8= armar_lista_offsets(id,64,18);
		log_info(logger,"Lista Error");

		void* datos2 = leer_datos(lista_bloques2);
		log_info(logger,"%s",datos2);

		void* datos4 = leer_datos(lista_bloques4);
		log_info(logger,"%s",datos4);

		t_list* lista_bloques9 = armar_lista_offsets(id,4,6);

		log_info(logger,"Lista 9");
		list_iterate(lista_bloques9,(void*) imprimir_bloques2);

		void* datos_ej = malloc(4);
		memcpy(datos_ej,"ABCD",4);

		escribir_datos(datos_ej,lista_bloques8);

	int idej = buscar_fcb("ParcialDamian");
	//int cant_bloques = 500 / 64;
	//modificar_fcb(idej, TAMANIO_ARCHIVO, 500);
	//asignar_bloques(idej, 512);

	//obtener_lista_de_bloques(idej);

	//desasignar_bloques(idej, 128);

	char* ejemplo2 = "QUIERO COPIAR TODO ESTO EN MI ARCHIVO";

	int bloque_directo = valor_fcb(idej,PUNTERO_DIRECTO);
	//memcpy(memoria_file_system + (bloque_directo * tamanio_de_bloque), ejemplo2, strlen(ejemplo2));
	int bloque_indirecto = valor_fcb(idej,PUNTERO_INDIRECTO);

	t_list* list_ejemplo = list_create();

	list_add(list_ejemplo,&bloque_directo);

	int offset3 = 0;

	/*for (int i = 0; i < cant_bloques; i++){
		void* s_numero = malloc(4);
		memcpy(s_numero,memoria_file_system + (bloque_indirecto * tamanio_de_bloque) + offset3,sizeof(uint32_t));
		offset3 += sizeof(uint32_t);
		//uint32_t numero = atoi(s_numero);
		list_add(list_ejemplo,s_numero);
	}

	for (int i = 0; i < list_size(list_ejemplo); i++){
		void* puntero_ejemplo = list_get(list_ejemplo,i);
		uint32_t bloque_ejemplo = atoi(puntero_ejemplo);
		memcpy(memoria_file_system + (bloque_ejemplo * tamanio_de_bloque), ejemplo2, strlen(ejemplo2));
	}*/

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


