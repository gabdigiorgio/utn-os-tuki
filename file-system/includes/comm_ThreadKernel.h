#ifndef COMM_THREADKERNEL_H_
#define COMM_THREADKERNEL_H_

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
#include "client_utils.h"
#include "server_utils.h"
#include "utils.h"
#include "comm_Mem.h"

extern t_log* logger;
extern int memoria_connection;

void comm_threadKernel(int kernel_connection);

#endif /* COMM_THREADKERNEL_H_ */
