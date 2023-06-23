#ifndef COMM_THREADCPU_H_
#define COMM_THREADCPU_H_

#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include<commons/log.h>
#include<commons/collections/list.h>

#include"client_utils.h"
#include"server_utils.h"
#include"datos.h"
#include"../../shared/includes/tad.h"

void conexion_cpu(int server_connection);

extern int exit_status;
extern t_log* logger;
extern void* memoria;

#endif /* COMM_THREADCPU_H_ */
