/*
 * initial_setup.c
 *
 *  Created on: Apr 6, 2023
 *      Author: utnso
 */

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
		failed_initial_setup("IP_MEMORIA");
		error = 0;
	}

	if (config_has_property(config, "PUERTO_MEMORIA"))
	{
		memoria_port = config_get_string_value(config, "PUERTO_MEMORIA");
	}
	else
	{
		failed_initial_setup("PUERTO_MEMORIA");
		error = 0;
	}

	// SERVER PORT
	if (config_has_property(config, "PUERTO_ESCUCHA"))
	{
		server_port = config_get_string_value(config, "PUERTO_ESCUCHA");
	}
	else
	{
		failed_initial_setup("PUERTO_ESCUCHA");
		error = 0;
	}

	// RETARDO
	if (config_has_property(config, "RETARDO_INSTRUCCION"))
	{
		retardo_instruc = config_get_int_value(config, "RETARDO_INSTRUCCION") / 1000;
	}
	else
	{
		failed_initial_setup("RETARDO_INSTRUCCION");
		error = 0;
	}

	// TAMANIO MAXIMO DE SEGMENTO
	if (config_has_property(config, "TAM_MAX_SEGMENTO"))
	{
		tam_max_segmento = config_get_int_value(config, "TAM_MAX_SEGMENTO");
	}
	else
	{
		failed_initial_setup("TAM_MAX_SEGMENTO");
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

void failed_initial_setup(char *key)
{
	char *info_error = string_new();
	string_append(&info_error, "No se pudo obtener del archivo config el valor de ");
	string_append(&info_error, key);
	log_error(logger, "%s", info_error);
}
