# Bienvenidos al TP Final Segmentation-Fault

## INSTRUCCIONES CUSTOM

- PRINT_SEGMENTS sin parametros
- PRINT_MEMORY_DATA sin parametros
- F_DELETE nombre_archivo
- PRINT_FILE_STATE sin parametros
- PRINT_FILE_DATA sin parametros

## Detalles del Repositorio

    Para trabajar en el repositorio vamos a utilizar la siguientes branchs:

    - main
    - dev
    - feature#000-detalle
    - bugfix#000-detalle

    Partir siempre de main para realizar un nuevo feature. Al terminar de trabajar, crear una nueva branch feature, con un número correlativo, y el nombre de lo que se este trabajando actualmente. IMPORTANTE!! Subir lo trabajado TODOS los días apenas se termina de trabajar, por mas que este incompleto o no funcione.

    No pushear a main ni a dev de ninguna manera. Nos vamos a manejar con merges.
    Una vez que el feature o el fix esté terminado, se hace un merge-request a dev y lo evaluamos en grupo. Si todo funciona en dev, se hace el merge-request en main para dejarlo listo para la entrega.

    En caso de tener que continuar sobre lo que trabajo un compañero porque x no pudo terminarlo/se enfermó/etc, pullear desde el feature correspondiente y crear uno nuevo.

## Consigna

    El TP final consiste en realizar una simulación de un sistema operativo. Para el mismo se utilizará la siguiente consigna:

https://docs.google.com/document/d/1xYmkJXRRddM51fQZfxr3CEuhNtFCWe5YU7hhvsUnTtg/edit#heading=h.srt0ekjc1b9g ( temporal )

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

## División de tareas

    Para comenzar hay que realizar las conecciones iniciales. Empezar por memoria -> cpu -> kernel -> consolas.
