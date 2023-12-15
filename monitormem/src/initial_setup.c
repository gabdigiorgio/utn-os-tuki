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
	if (config_has_property(config, "TAM_LINEA")){
		tam_linea = config_get_int_value(config, "TAM_LINEA");
	} else {
		failed_initial_setup("TAM_LINEA");
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
