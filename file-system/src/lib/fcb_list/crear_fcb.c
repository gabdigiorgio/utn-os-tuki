#include "../../../includes/fcb_list.h"

int crear_fcb(char* nombre_fcb){
	int resultado = -1;

	if(buscar_fcb(nombre_fcb) != -1){
		log_error(logger,"Ya existe un FCB con ese nombre");
		return resultado;
	}

	char* nombre_completo = string_new();
	string_append(&nombre_completo,"./././fcb/");
	string_append(&nombre_completo,nombre_fcb);
	string_append(&nombre_completo,".dat");

	fcb_t* nuevo_fcb = inicializar_fcb();
	nuevo_fcb->id = fcb_id++;
	nuevo_fcb->nombre_archivo = string_new();
	string_append(&nuevo_fcb->nombre_archivo,nombre_fcb);
	nuevo_fcb->ruta_archivo = string_new();
	string_append(&nuevo_fcb->ruta_archivo,nombre_completo);

	t_config* fcb_fisico = malloc(sizeof(t_config));
	fcb_fisico->path = string_new();
	fcb_fisico->properties = dictionary_create();
	string_append(&fcb_fisico->path,nombre_completo);
	char* nombre_duplicado = string_duplicate(nombre_fcb);

	dictionary_put(fcb_fisico->properties,"NOMBRE_ARCHIVO", nombre_duplicado);
	dictionary_put(fcb_fisico->properties,"TAMANIO_ARCHIVO", "0");
	dictionary_put(fcb_fisico->properties,"PUNTERO_DIRECTO", "0");
	dictionary_put(fcb_fisico->properties,"PUNTERO_INDIRECTO", "0");
	dictionary_put(fcb_fisico->properties,"PUNTERO_ARCHIVO", "0");

	config_save_in_file(fcb_fisico,nombre_completo);
	list_add(lista_global_fcb->lista_fcb,nuevo_fcb);
	dictionary_destroy(fcb_fisico->properties);
	free(nombre_duplicado);
	free(fcb_fisico->path);
	free(fcb_fisico);
	free(nombre_completo);

	return nuevo_fcb->id;
}
