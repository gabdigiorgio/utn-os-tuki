/*
 * initial_setup.c
 *
 *  Created on: Apr 6, 2023
 *      Author: utnso
 */

#include "../includes/initial_setup.h"


int initial_setup(){
	int error = 1;

	// SERVER PORT
	if (config_has_property(config, "PUERTO_ESCUCHA")){
		server_port = config_get_string_value(config, "PUERTO_ESCUCHA");
	} else {
		failed_initial_setup("PUERTO_ESCUCHA");
		error = 0;
	}

	// TAM_MEMORIA
	if (config_has_property(config, "TAM_MEMORIA")){
		tam_memoria = config_get_int_value(config, "TAM_MEMORIA");
	} else {
		failed_initial_setup("TAM_MEMORIA");
		error = 0;
	}

	// TAM_SEGMENTO_0
	if (config_has_property(config, "TAM_SEGMENTO_0")){
		tam_segmento_0 = config_get_int_value(config, "TAM_SEGMENTO_0");
	} else {
		failed_initial_setup("TAM_SEGMENTO_0");
		error = 0;
	}

	// CANT_SEGMENTOS
	if (config_has_property(config, "CANT_SEGMENTOS")){
		cant_segmentos = config_get_int_value(config, "CANT_SEGMENTOS");
	} else {
		failed_initial_setup("CANT_SEGMENTOS");
		error = 0;
	}

	// RETARDO_MEMORIA
	if (config_has_property(config, "RETARDO_MEMORIA")){
		retardo_memoria = config_get_int_value(config, "RETARDO_MEMORIA");
	} else {
		failed_initial_setup("RETARDO_MEMORIA");
		error = 0;
	}

	// RETARDO_COMPACTACION
	if (config_has_property(config, "RETARDO_COMPACTACION")){
		retardo_compactacion = config_get_int_value(config, "RETARDO_COMPACTACION");
	} else {
		failed_initial_setup("RETARDO_COMPACTACION");
		error = 0;
	}

	// ALGORITMO_ASIGNACION
	if (config_has_property(config, "ALGORITMO_ASIGNACION")){
		algoritmo_asignacion = config_get_string_value(config, "ALGORITMO_ASIGNACION");
	} else {
		failed_initial_setup("ALGORITMO_ASIGNACION");
		error = 0;
	}




	if(error == 1){
		log_info(logger, "Valores de configuracion leidos correctamente");
		return EXIT_SUCCESS;
	} else {
		return EXIT_FAILURE;
	}

}

void failed_initial_setup(char* key){
	char *info_error = string_new();
	string_append(&info_error, "No se pudo obtener del archivo config el valor de ");
	string_append(&info_error, key);
	log_error(logger, "%s",info_error);
}
