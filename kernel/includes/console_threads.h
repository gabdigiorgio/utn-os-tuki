#ifndef CONSOLE_THREADS_H_
#define CONSOLE_THREADS_H_

#include <commons/string.h>

#include "console_threads.h"
#include "server_utils.h"
#include "client_utils.h"

void atender_consola(int *socket_console_client);
void iterator(char* value);


extern t_log* logger;
extern int cant_threads_activos;;

#endif /* CONSOLE_THREADS_H_ */
