#include "../includes/initial_setup.h"


int initial_setup(){
	int error = 1;

	//KERNEL
	
	if (config_has_property(config, "IP_KERNEL")){
		kernel_ip = config_get_string_value(config, "IP_KERNEL");
	} else {
		failed_initial_setup("IP_KERNEL");
		error = 0;
	}

	if (config_has_property(config, "PUERTO_KERNEL")){
		kernel_port = config_get_string_value(config, "PUERTO_KERNEL");
	} else {
		failed_initial_setup("PUERTO_KERNEL");
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
