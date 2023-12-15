#include "../../includes/utils.h"

t_config* iniciar_config(char * path_config)
{
	t_config* nuevo_config;
	if((nuevo_config = config_create(path_config))==NULL){
		printf("No pude leer la config \n");
		exit(2);
	}
	return nuevo_config;
}

t_log* iniciar_logger(void)
{
	t_log* nuevo_logger = log_create("memoria.log", "MEMORIA", 1, LOG_LEVEL_DEBUG);

	return nuevo_logger;
}

void imprimir_datos(void* memoria, int tam_memoria){
	int offset = tam_memoria;
	int lineas = tam_memoria / tam_linea;
	int b = 0;
	char string[tam_memoria + lineas + 1];
	char nueva_linea = '\n';
	char final = '\0';

	for(int i = 0; i<(offset + lineas); i++){
		if((i % tam_linea) == 0){
			memcpy(string + i,&nueva_linea, sizeof(char));
			continue;
		}
		memcpy(string + i,memoria + b, sizeof(char));
		if(string[i] == '\0') string[i] = '0';
		b++;
	}
	memcpy(string + (offset + lineas),&final, sizeof(char));
	printf("\e[1;1H\e[2J");
	printf("%s",string);
}
