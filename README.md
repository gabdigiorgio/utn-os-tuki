# Bienvenidos al TP Final Segmentation-Fault

## INSTRUCCIONES CUSTOM

- PRINT_SEGMENTS sin parametros
- PRINT_MEMORY_DATA sin parametros
- F_DELETE nombre_archivo
- PRINT_FILE_STATE sin parametros
- PRINT_FILE_DATA sin parametros

## Consigna

    El TP final consiste en realizar una simulación de un sistema operativo. Para el mismo se utilizará la siguiente consigna:

https://docs.google.com/document/d/1orfThJsPmMx5uPzbY3wClGhqX8jASMOCUMlWnYAr7cA/edit

    Se deberán crear 4 módulos de consola, un módulo de Kernel, un módulo de CPU y un módulo de Memoria, cada uno con su propia instancia.

### Consolas

    Cada una consiste en 1 archivo de configuración y 1 archivo de pseudocódigo con las instrucciones a ejecutar y el programa principal.

    Las mismas se utilizarán para conectarse a distintos módulos.

### Kernel

    Módulo que deberá mantener 2 conecciones con CPU (dispatch / interrupt) y 1 con la memoria. Ademas puede recibir conecciones de las diferentes consolas ( max 4 ).

### CPU

    Al iniciar se conecta a memoria y realiza un handshake. Espera conecciones de Kernel por alguno de los puertos (dispath / interrupt)

### Memoria

    Recibira conección desde CPU.

### Protocolo de handshake

    Cada módulo del sistema envía un mensaje de handshake al servidor al conectarse. El mensaje de handshake tiene un valor entero que indica el tipo de módulo:

    - 1: CPU
    - 2: Kernel
    - 3: File System
    - 4: Consola

    Si el servidor recibe un valor mayor a 0, asume que se trata de un mensaje de handshake. El servidor responde con un valor entero que indica si el handshake fue exitoso:

    - 1: Handshake exitoso
    - -1: Error de handshake
