#include "../includes/initial_setup.h"

int initial_setup()
{
	int error = 1;

	//Memoria

	if (config_has_property(config, "IP_MEMORIA"))
	{
		memoria_ip = config_get_string_value(config, "IP_MEMORIA");
	}
	else
	{
		failed_setup("IP_MEMORIA");
		error = 0;
	}

	if (config_has_property(config, "PUERTO_MEMORIA"))
	{
		memoria_port = config_get_string_value(config, "PUERTO_MEMORIA");
	}
	else
	{
		failed_setup("PUERTO_MEMORIA");
		error = 0;
	}

	// SERVER PORT
	if (config_has_property(config, "PUERTO_ESCUCHA"))
	{
		server_port = config_get_string_value(config, "PUERTO_ESCUCHA");
	}
	else
	{
		failed_setup("PUERTO_ESCUCHA");
		error = 0;
	}

	// PATH_SUPERBLOQUE
	if (config_has_property(config, "PATH_SUPERBLOQUE"))
	{
		path_superbloque = config_get_string_value(config, "PATH_SUPERBLOQUE");
	}
	else
	{
		failed_setup("PATH_SUPERBLOQUE");
		error = 0;
	}

	// PATH_BITMAP
	if (config_has_property(config, "PATH_BITMAP"))
	{
		path_bitmap = config_get_string_value(config, "PATH_BITMAP");
	}
	else
	{
		failed_setup("PATH_BITMAP");
		error = 0;
	}

	// PATH_BLOQUES
	if (config_has_property(config, "PATH_BLOQUES"))
	{
		path_bloques = config_get_string_value(config, "PATH_BLOQUES");
	}
	else
	{
		failed_setup("PATH_BLOQUES");
		error = 0;
	}

	// PATH_FCB
	if (config_has_property(config, "PATH_FCB"))
	{
		path_fcb_folder = config_get_string_value(config, "PATH_FCB");
	}
	else
	{
		failed_setup("PATH_FCB");
		error = 0;
	}

	// RETARDO_ACCESO_BLOQUE
	if (config_has_property(config, "RETARDO_ACCESO_BLOQUE"))
	{
		retardo_acceso_bloque = config_get_int_value(config, "RETARDO_ACCESO_BLOQUE");
	}
	else
	{
		failed_setup("RETARDO_ACCESO_BLOQUE");
		error = 0;
	}

	if (error == 1)
	{
		log_info(logger, "Valores de configuracion leidos correctamente");
		return EXIT_SUCCESS;
	}
	else
	{
		return EXIT_FAILURE;
	}

}

int crear_bitmap()
{
	int tamanio_bitmap = cantidad_de_bloques / (tamanio_de_bloque * 8); // Usar ceil()

	int file_descriptor = open(path_bitmap, O_CREAT | O_RDWR, 0644); // 0644 -> permisos de lectura/escritura
	if (file_descriptor < 0)
	{
		log_error(logger, "No se pudo crear el archivo de bitmap");
		return EXIT_FAILURE;
	}

	ftruncate(file_descriptor, tamanio_bitmap);

	void *bitmap_data = mmap(NULL, tamanio_bitmap, PROT_READ | PROT_WRITE, MAP_SHARED, file_descriptor, 0);
	if (bitmap_data == MAP_FAILED)
	{
		log_error(logger, "No se pudo mapear el archivo de bitmap");
		close(file_descriptor);
		return EXIT_FAILURE;
	}

	memset(bitmap_data, 0, tamanio_bitmap); // Inicializo los bits en 0

	bitmap = bitarray_create_with_mode(bitmap_data, tamanio_bitmap, LSB_FIRST);

	return EXIT_SUCCESS;
}

int leer_superbloque_config()
{
	int error = 1;

	// BLOCK_SIZE
	if (config_has_property(superbloque_config, "BLOCK_SIZE"))
	{
		tamanio_de_bloque = config_get_int_value(superbloque_config, "BLOCK_SIZE");
	}
	else
	{
		failed_setup("BLOCK_SIZE");
		error = 0;
	}

	// BLOCK_COUNT
	if (config_has_property(superbloque_config, "BLOCK_COUNT"))
	{
		cantidad_de_bloques = config_get_int_value(superbloque_config, "BLOCK_COUNT");
	}
	else
	{
		failed_setup("BLOCK_COUNT");
		error = 0;
	}

	if (error == 1)
	{
		log_info(logger, "Valores de configuracion leidos correctamente");
		return EXIT_SUCCESS;
	}
	else
	{
		return EXIT_FAILURE;
	}

}

void failed_setup(char *key)
{
	char *info_error = string_new();
	string_append(&info_error, "No se pudo obtener del archivo el valor de ");
	string_append(&info_error, key);
	log_error(logger, "%s", info_error);
}
