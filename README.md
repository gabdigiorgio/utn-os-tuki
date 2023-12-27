# Bienvenidos al TP Final Segmentation-Fault / Welcome to the Final Project Segmentation-Fault

## INSTRUCCIONES CUSTOM / CUSTOM INSTRUCTIONS

- PRINT_SEGMENTS sin parametros / without parameters
- PRINT_MEMORY_DATA sin parametros / without parameters
- F_DELETE nombre_archivo / file_name
- PRINT_FILE_STATE sin parametros / without parameters
- PRINT_FILE_DATA sin parametros / without parameters

## Consigna / Assignment

    El TP final consiste en realizar una simulación de un sistema operativo. Para el mismo se utilizará la siguiente consigna:
    The final project involves creating a simulation of an operating system. The following guidelines will be used for this purpose:

https://docs.google.com/document/d/1orfThJsPmMx5uPzbY3wClGhqX8jASMOCUMlWnYAr7cA/edit

    Se deberán crear 4 módulos de consola, un módulo de Kernel, un módulo de CPU y un módulo de Memoria, cada uno con su propia instancia.
    Four console modules, one kernel module, one CPU module, and one memory module should be created, each with its own instance.

### Consolas / Consoles

    Cada una consiste en 1 archivo de configuración y 1 archivo de pseudocódigo con las instrucciones a ejecutar y el programa principal.
    Each module consists of one configuration file and one pseudocode file with the instructions to be executed and the main program.

    Las mismas se utilizarán para conectarse a distintos módulos.
    These files will be used to connect to different modules.

### Kernel

    Módulo que deberá mantener 2 conecciones con CPU (dispatch / interrupt) y 1 con la memoria. Ademas puede recibir conecciones de las diferentes consolas ( max 4 ).
    Module that should maintain 2 connections with CPU (dispatch/interrupt) and 1 with memory. Additionally, it can receive connections from different consoles (maximum 4).

### CPU

    Al iniciar se conecta a memoria y realiza un handshake. Espera conecciones de Kernel por alguno de los puertos (dispath / interrupt).
    Upon initiation, it connects to memory and performs a handshake. It waits for connections from the Kernel through either of the ports (dispatch/interrupt).

### Memoria / Memory

    Recibira conexión desde CPU.
    It will receive a connection from the CPU.

### Protocolo de handshake / Handshake protocol

    Cada módulo del sistema envía un mensaje de handshake al servidor al conectarse. El mensaje de handshake tiene un valor entero que indica el tipo de módulo:
    Each module in the system sends a handshake message to the server upon connection. The handshake message contains an integer value indicating the type of module:

    - 1: CPU
    - 2: Kernel
    - 3: File System
    - 4: Consola / Console

    Si el servidor recibe un valor mayor a 0, asume que se trata de un mensaje de handshake. El servidor responde con un valor entero que indica si el handshake fue exitoso:
    If the server receives a value greater than 0, it assumes it is a handshake message. The server responds with an integer value indicating whether the handshake was successful:

    - 1: Handshake exitoso / Successful handshake
    - -1: Error de handshake / Handshake error
