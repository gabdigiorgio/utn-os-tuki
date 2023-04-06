/*
 * memoria.h
 *
 *  Created on: Apr 3, 2023
 *      Author: utnso
 */

#ifndef CPU_H_
#define CPU_H_

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <commons/log.h>
#include <commons/string.h>
#include <commons/config.h>
#include <readline/readline.h>

#include "server_utils.h"
#include "client_utils.h"
#include "utils.h"

void iterator(char* value);
char* initial_setup(void);

#endif /* CPU_H_ */
