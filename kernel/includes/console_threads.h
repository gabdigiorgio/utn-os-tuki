/*
 * threads.h
 *
 *  Created on: Apr 7, 2023
 *      Author: utnso
 */

#ifndef CONSOLE_THREADS_H_
#define CONSOLE_THREADS_H_

#include "console_threads.h"

#include "server_utils.h"
#include "client_utils.h"

void atender_consola(int *socket_console_client);
void iterator(char* value);
extern t_log* logger;

#endif /* CONSOLE_THREADS_H_ */
