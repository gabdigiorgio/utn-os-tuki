#include "../../../includes/fcb_list.h"

void inicializar_fcb_list(){
	lista_global_fcb = malloc(sizeof(fcb_list_t));
	lista_global_fcb->lista_fcb = list_create();
	char* directorio = "./././fcb/";

	DIR* dir_fcb = opendir(directorio);

	struct dirent* dir_entry;

	while ((dir_entry = readdir(dir_fcb)) != NULL){
		if(strcmp(dir_entry->d_name,".") == 0) continue;
		if(strcmp(dir_entry->d_name,"..") == 0) continue;
		int error = 0;
		fcb_t* fcb_existente = malloc(sizeof(fcb_t));

		fcb_existente->nombre_archivo = string_new();

		char* nombre_completo = string_new();
		string_append(&nombre_completo,directorio);
		string_append(&nombre_completo,dir_entry->d_name);

		t_config* fcb = config_create(nombre_completo);
		if(config_has_property(fcb, "NOMBRE_ARCHIVO")) string_append(&fcb_existente->nombre_archivo,config_get_string_value(fcb,"NOMBRE_ARCHIVO"));
		else error = 1;
		if(config_has_property(fcb, "TAMANIO_ARCHIVO")) fcb_existente->tamanio_archivo = (uint32_t) config_get_int_value(fcb,"TAMANIO_ARCHIVO");
		else error = 1;
		if(config_has_property(fcb, "PUNTERO_DIRECTO")) fcb_existente->puntero_directo = (uint32_t) config_get_int_value(fcb,"PUNTERO_DIRECTO");
		else error = 1;
		if(config_has_property(fcb, "PUNTERO_INDIRECTO")) fcb_existente->puntero_indirecto = (uint32_t) config_get_int_value(fcb,"PUNTERO_INDIRECTO");
		else error = 1;
		if(config_has_property(fcb, "PUNTERO_ARCHIVO")) fcb_existente->puntero_archivo = (uint32_t) config_get_int_value(fcb,"PUNTERO_ARCHIVO");
		else error = 1;
		fcb_existente->ruta_archivo = string_new();
		string_append(&fcb_existente->ruta_archivo,nombre_completo);
		fcb_existente->id = fcb_id;

		if(error == 0){
			list_add(lista_global_fcb->lista_fcb,fcb_existente);
			fcb_id++;
		}
		else log_error(logger,"Formato incorrecto de FCB");

		free(nombre_completo);
		config_destroy(fcb);
	}

	free(dir_entry);
	closedir(dir_fcb);
}
