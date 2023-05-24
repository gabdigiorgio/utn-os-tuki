/*
 * initial_setup.c
 *
 *  Created on: Apr 6, 2023
 *      Author: utnso
 */

#include "../includes/initial_setup.h"

int initial_setup(){
	int error = 1;

	//Memoria
	
	if (config_has_property(config, "IP_MEMORIA")){
		memoria_ip = config_get_string_value(config, "IP_MEMORIA");
	} else {
		failed_initial_setup("IP_MEMORIA");
		error = 0;
	}

	if (config_has_property(config, "PUERTO_MEMORIA")){
		memoria_port = config_get_string_value(config, "PUERTO_MEMORIA");
	} else {
		failed_initial_setup("PUERTO_MEMORIA");
		error = 0;
	}

	//File System

	if (config_has_property(config, "IP_FILESYSTEM")){
		file_system_ip = config_get_string_value(config, "IP_FILESYSTEM");
	} else {
		failed_initial_setup("IP_FILESYSTEM");
		error = 0;
	}

	if (config_has_property(config, "PUERTO_FILESYSTEM")){
		file_system_port = config_get_string_value(config, "PUERTO_FILESYSTEM");
	} else {
		failed_initial_setup("PUERTO_FILESYSTEM");
		error = 0;
	}

	//CPU
	if (config_has_property(config, "IP_CPU")){
		cpu_ip = config_get_string_value(config, "IP_CPU");
	} else {
		failed_initial_setup("IP_CPU");
		error = 0;
	}

	if (config_has_property(config, "PUERTO_CPU")){
		cpu_port = config_get_string_value(config, "PUERTO_CPU");
	} else {
		failed_initial_setup("PUERTO_CPU");
		error = 0;
	}

	// SERVER PORT
	if (config_has_property(config, "PUERTO_ESCUCHA")){
		server_port = config_get_string_value(config, "PUERTO_ESCUCHA");
	} else {
		failed_initial_setup("PUERTO_ESCUCHA");
		error = 0;
	}

	// ALGORITMO PLANIFICACION
	if (config_has_property(config, "ALGORITMO_PLANIFICACION")){
		algoritmo_planificacion = config_get_string_value(config, "ALGORITMO_PLANIFICACION");
		} else {
			failed_initial_setup("ALGORITMO_PLANIFICACION");
			error = 0;
		}

	// ESTIMACION_INICIAL
	if (config_has_property(config, "ESTIMACION_INICIAL")){
		estimacion_inicial = config_get_int_value(config, "ESTIMACION_INICIAL");
		} else {
			failed_initial_setup("ESTIMACION_INICIAL");
			error = 0;
		}

	// HRRN_ALFA
	if (config_has_property(config, "HRRN_ALFA")){
		hrrn_alfa = config_get_double_value(config, "HRRN_ALFA");
		} else {
			failed_initial_setup("HRRN_ALFA");
			error = 0;
		}

	// GRADO_MAX_MULTIPROGRAMACION
	if (config_has_property(config, "GRADO_MAX_MULTIPROGRAMACION")){
		grado_max_multiprogramacion = config_get_int_value(config, "GRADO_MAX_MULTIPROGRAMACION");
		} else {
			failed_initial_setup("GRADO_MAX_MULTIPROGRAMACION");
			error = 0;
		}

	// RECURSOS
	if (config_has_property(config, "RECURSOS")){
		lista_nombre_recursos = string_array_new();
		lista_nombre_recursos = config_get_array_value(config, "RECURSOS");
		} else {
			failed_initial_setup("RECURSOS");
			error = 0;
		}
	if (config_has_property(config, "INSTANCIAS_RECURSOS")){
		lista_instancias_recursos = string_array_new();
		lista_instancias_recursos = config_get_array_value(config, "INSTANCIAS_RECURSOS");
		} else {
			failed_initial_setup("INSTANCIAS_RECURSOS");
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
