#ifndef CONSOLE_THREADS_H_
#define CONSOLE_THREADS_H_

#include <commons/string.h>
#include <commons/collections/queue.h>

#include "server_utils.h"
#include "client_utils.h"
#include "../../shared/includes/tad.h"

void atender_consola(int *socket_console_client);
void iterator(char* value);

extern t_log* logger;
extern int cant_threads_activos;;
extern int cpu_connection;

#endif /* CONSOLE_THREADS_H_ */
