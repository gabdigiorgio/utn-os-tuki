#ifndef COMM_THREADMEM_H_
#define COMM_THREADMEM_H_

#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include<commons/log.h>
#include<commons/collections/list.h>

#include "server_utils.h"
#include "utils.h"
#include "mem_instruct.h"
#include "algoritmos.h"
#include "client_utils.h"
#include "../../shared/includes/tad.h"

void conexion_file_system(int server_connection);

extern t_log* logger;


#endif /* COMM_THREADMEM_H_ */
