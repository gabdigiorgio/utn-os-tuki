/*
 * initial_setup.c
 *
 *  Created on: Apr 6, 2023
 *      Author: utnso
 */

#include "../includes/initial_setup.h"


char* initial_setup(){
	char* result = "";
	int error = 1;

	//Memoria
	if(error == 1 && strcmp(memoria_ip = config_get_string_value(config,"IP_MEMORIA"),"") == 0){
		result = "No se pudo obtener la IP desde el archivo config";
		error = 0;
	}
	if(error == 1 && strcmp(memoria_port = config_get_string_value(config,"PUERTO_MEMORIA"),"") == 0){
		result = "No se pudo obtener el puerto de conexion desde el archivo config";
		error = 0;
	}

	//File System
	if(error == 1 && strcmp(file_system_ip = config_get_string_value(config,"IP_FILESYSTEM"),"") == 0){
		result = "No se pudo obtener la IP desde el archivo config";
		error = 0;
	}
	if(error == 1 && strcmp(file_system_port = config_get_string_value(config,"PUERTO_FILESYSTEM"),"") == 0){
		result = "No se pudo obtener el puerto de conexion desde el archivo config";
		error = 0;
	}

	//CPU
	if(error == 1 && strcmp(cpu_ip = config_get_string_value(config,"IP_CPU"),"") == 0){
		result = "No se pudo obtener la IP desde el archivo config";
		error = 0;
	}
	if(error == 1 && strcmp(cpu_port = config_get_string_value(config,"PUERTO_CPU"),"") == 0){
		result = "No se pudo obtener el puerto de conexion desde el archivo config";
		error = 0;
	}


	// SERVER PORT
	if(error == 1 && strcmp(server_port = config_get_string_value(config,"PUERTO_ESCUCHA"),"") == 0){
		result = "No se pudo obtener el puerto de escucha desde el archivo config";
		error = 0;
	}


	if(error == 1) result = "Valores de configuracion leidos correctamente";

	return result;
}
