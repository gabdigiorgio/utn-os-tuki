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


	// SERVER PORT
	if (config_has_property(config, "PUERTO_ESCUCHA")){
		server_port = config_get_string_value(config, "PUERTO_ESCUCHA");
	} else {
		failed_initial_setup("PUERTO_ESCUCHA");
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
