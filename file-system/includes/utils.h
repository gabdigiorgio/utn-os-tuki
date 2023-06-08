#ifndef UTILS_H_
#define UTILS_H_

#include "../../shared/includes/tad.h"
#include<stdio.h>
#include<stdlib.h>
#include<stdbool.h>
#include<commons/log.h>
#include<commons/string.h>
#include<commons/config.h>
#include<readline/readline.h>
#include <commons/collections/list.h>
#include <pthread.h>

t_log* iniciar_logger(void);
fcb_t* iniciar_fcb(char* path_fcb);
t_config* iniciar_config(char * path_config);
#endif /* CLIENT_H_ */
