# CUESTIONARIO


## Ejercicio No 1: Display LCD

**a) Vamos a utilizar el LCD 1602A de 16 caracteres y dos líneas para visualizar información. Investigue con la hoja de datos como funciona este periférico, realice un resumen de sus características y de los comandos necesarios para su operación.**

| Pin No. | Symbol | Level | Description                                                                                                                                                |
| ------: | ------ | ----- | ---------------------------------------------------------------------------------------------------------------------------------------------------------- |
|       1 | VSS    | 0V    | Ground.                                                                                                                                                    |
|       2 | VDD    | +5.0V | Power supply for logic operating.                                                                                                                          |
|       3 | VO     | --    | Adjusting supply voltage for LCD driving.                                                                                                                  |
|       4 | RS     | H/L   | A signal for selecting registers: <br>1: Data Register (for read and write) <br>0: Instruction Register (for write), Busy flag-Address Counter (for read). |
|       5 | R/W    | H/L   | R/W = “H”: Read mode. <br>R/W = “L”: Write mode.                                                                                                           |
|       6 | E      | H/L   | An enable signal for writing or reading data.                                                                                                              |
|       7 | DB0    | H/L   |                                                                                                                                                            |
|       8 | DB1    | H/L   |                                                                                                                                                            |
|       9 | DB2    | H/L   |                                                                                                                                                            |
|      10 | DB3    | H/L   | This is an 8-bit bi-directional data bus.                                                                                                                  |
|      11 | DB4    | H/L   |                                                                                                                                                            |
|      12 | DB5    | H/L   |                                                                                                                                                            |
|      13 | DB6    | H/L   |                                                                                                                                                            |
|      14 | DB7    | H/L   |                                                                                                                                                            |
|      15 | LED+   | +5.0V | Power supply for backlight.                                                                                                                                |
|      16 | LED-   | 0V    | The backlight ground.                                                                                                                                      |



**b) Haga el diagrama esquemático en Proteus que incluya el LCD y el teclado matricial del kit utilizado en clase (fig.1)**

![Proteus](/Practicas/Practica2/image.png)


**c) Verifique que los niveles lógicos de la interfaz del periférico sean compatibles con los niveles de los puertos I/O del MCU.**

Los niveles de voltaje de la interfaz del periférico son:

|        Parameter        | Symbol | Condition |   Min   |  TYP  |   Max   | Unit  |
| :---------------------: | :----: | :-------: | :-----: | :---: | :-----: | :---: |
|  Input voltage“H”level  |  VIH   |    --     | 0.7 VDD |  --   | VDD+0.3 |   V   |
| Input voltage “L” level |  VIL   |    --     |    0    |  --   | 0.2VDD  |   V   |

Siendo VDD=5V entonces, dado que los niveles de los puertos I/O del Atmega 328p son 0V para el nivel bajo y 5V para el alto, los niveles lógicos son compatibles.


**d) Explique cómo funcionan las funciones de biblioteca para el control del LCD que utilizaremos en la práctica (provistas en los archivos *lcd.h* y *lcd.c*). Particularmente, las funciones: ``LCDinit()``, ``LCDgotoxy()`` y ``LCDstring()``.**

- ``LCDinit(void)``: Setea los 4 puertos I/O involucrados en el trabajo con el LCD, siguiendo la configuración de la (fig.1) en caso de definir previamente la macro ``LCD_4bit``. Sino configura 8 puertos, suponiendo que se trabaja con DB0 a DB7 del LCD. Además, setea el display como encendido, y el cursor y el parpadeo como apagados.
- ``LCDGotoxy(uint8_t x, uint8_t y)``: Mueve el cursor a la posición (x,y).
- ``LCDstring()``: Envía un string al LCD.




## Ejercicio No 2: Teclado Matricial 4x4

**a) Explique cómo funciona el teclado de membrana dispuesto en forma matricial (4x4). Realice un esquema de conexionado para controlarlo con los puertos I/O del MCU. ATENCION: el diagrama de conexionado será el del kit utilizado en clase (fig.1) Explique cómo configurar la dirección de cada teminal I/O y cuales requieren resistencias de Pull-ups.**

El teclado de membrana matricial de 4x4 funciona de la siguiente manera: El teclado esta conformado por 4 filas (cada una corresponde a una salida del MCU) y 4 columnas (cada una corresponde a una entrada del MCU), cuando se presiona una de las teclas del teclado se activa un pulsador que conecta la fila y columna en la cual esta posicionado. De esta forma, la salida de una fila queda como entrada de la columna. De esta forma, las entradas y salidas pueden configurarse de la siguiente forma:
- Entradas (representan columnas) con resistencia de pull-up.
- Salidas en bajo, aquellas salidas que no tengan botones presionados en la fila correspondiente quedan en alta impedancia.



**b) Realice una función en C con el siguiente prototipo uint8 KEYPAD_Scan(uint8 \*key), que permita encuestar el teclado para determinar si se presionó una tecla y el valor de la misma. La función debe retornar 0 si no se presionó ninguna tecla o 1 si se presionó alguna. El valor de la tecla (o carácter ASCII) debe retornarse por el puntero pasado como parámetro (\*key).**

```c
uint8_t KEYPAD_Scan(uint8 *key);
uint8_t KePadUpdate(void);

uint8_t KePadUpdate(void)
{
  uint8_t r, c;
  PORTB |= 0x0F;
  for(c=0; c<4; c++) 
  {
    DDRB &=~(0XFF);
    DDRB|=(0X80>>c);
    for(r=0; r<4; r++) 
    {
      if(!(PINB & (0X80>>r)))
      {
        return (r*4+c);
      }
    }
  }
  return 0XFF; //tecla No presionada
}

uint8_t KEYPAD_Scan (uint8_t *pkey)
{
  static uint8_t Old_key, Last_valid_key=0xFF; // no hay tecla presionada;
  uint8_t Key,
  Key= KepadUpdate();
  if(Key==0xFF){
    Old_key=0xFF; // no hay tecla presionada
    Last_valid_key=0xFF;
    return 0;
  }
  if(Key==Old_key) { //2da verificación
    if(Key!=Last_valid_key){ //evita múltiple detección
      *pkey=Key;
      Last_valid_key = Key;
      return 1;
    }
  }
  Old_key=Key; //1era verificación
  return 0;
}
```


## Ejercicio No 3: Interrupciones

**a) Explique cuál es la secuencia de pasos que realiza el MCU cuando recibe una señal de pedido de interrupción. ¿cómo se retorna a la ejecución normal del programa?**

Cuando el MCU recibe una señal del pedido de interrupción el procedimiento es el siguiente:

1. Cuando un pedido de interrupción es reconocido y aceptado, la CPU finaliza la instrucción que estaba ejecutando y almacena el contador de programa PC (que apunta a la siguiente instrucción) en la PILA (indicada por el SP – stack pointer). Esto sucede si el bit I (máscara global de interrupciones) esta en 1.
2. Se deshabilita la máscara global I (I=0).
3. Según el número de interrupción que se haya generado, se busca en la Tabla de Vectores de Interrupción la instrucción que permite realizar el salto (implica cambiar el Program Counter) a la RSI (Rutina de ejecución de la interrupción) que corresponda ejecutar.
4. La CPU ejecuta la RSI hasta que encuentra la intrucción RETI, que indica el final de la misma.
5. Al ejecutar la instrucción RETI, el Program Counter se vuelve a cargar con el valor que tenía previo a la interrupción (para ello se desapila el valor del stack). Se sigue con la ejecución normal.


**b) Explique para qué sirve el bit I que se encuentra en el SREG. ¿Cuál es el valor por defecto luego del RESET? ¿Cuáles son las instrucciones que permiten modificarlo?**

El Bit I que se encuentra en el registro SREG (Status Register) es conocido como máscara global de interrupciones, y permite habilitar (con '1') o deshabilitar (con '0') la atención de las interrupciones por parte de la CPU. Su valor inicial es de '0', lo cual indica que inicialmente se encuentra deshabilitado. Las instrucciones que permiten modificarlo son ``cli()`` (deshabilita las interrupciones) y ``sei()`` (habilita las interrupciones).


**c) Explique que es un RESET, las distintas maneras por las que puede generarse y la función del registro MCU Status Register.**

El Reset es un evento que permite inicializar un sistema en un estado por defecto perfectamente conocido. Desde el punto de vista físico, no es más que una señal. El mismo puede generarse a partir de 4 eventos distintos:
- Power On: Un circuito genera una señal de reset cuando el sistema enciende.
- Reset externo: es un terminal donde el usuario puede colocar un pulsador o el sistema de programación y depuración.
- Watchdog: temporizador capaz de generar un reset si no se representa en periodo de tiempo determinado.
- Brown out reset: es un circuito que genera un reset si la tensión de alimentación VCC está fuera de los márgenes seguros de operación.

El registro MCU Status Register es quien almacena la causa del reset.

**d) Explique qué es la latencia de una interrupción y cuáles son los valores mínimos que puede tomar medida en ciclos de reloj.**

La latencia de una interrupción es el tiempo que tarda el Controlador de interrupciones en dar respuesta a tal interrupción. Es decir, es el tiempo desde que se recibe el pedido de interrupción hasta que se ejecuta la primera instrucción del RSI correspondiente. Como mínimo su valor es de 4 ciclos de reloj producto de todos los pasos que se deben realizar entre un evento y otro: Guardar el PC en la pila, I=0, buscar en el vector de interrupciones y realizar el salto.

**e) Explique qué entiende por interrupciones anidadas. ¿Es posible por defecto?**

El anidamiento de interrupciones es el proceso mediante el cuál se permite la interrupción cuando se esta tratando otra interrupción. Puesto que la máscara global de interrupciones I se deshabilita antes de tratar la interrupción, no es posible el anidamiento por defecto, para permitirlo se debe volver a habilitar las interrupciones dentro de la RSI que se está ejecutando.  


**f) Investigue sobre la configuración y el uso de los terminales de interrupción Externa (INTx). ¿Cuál es la diferencia entre configurar la interrupción por flanco o por nivel?**

El MCU tiene dos terminales que permiten interrupciones externas:

- **INT0** => PD2  
- **INT1** => PD3

Registros Clave

- `EIMSK`: Habilita interrupciones externas.
- `EIFR`: Banderas de interrupción.
- `EICRA`: Configura flanco/nivel.

Modos de activación (INT0 - ISC01/ISC00)

| ISC01 | ISC00 | Modo             |
| ----- | ----- | ---------------- |
| 0     | 0     | Nivel bajo       |
| 0     | 1     | Cualquier flanco |
| 1     | 0     | Flanco de bajada |
| 1     | 1     | Flanco de subida |

La diferencia entre Flanco vs Nivel es que 

| Tipo   | Activa cuando...    | Nota                                 |
| ------ | ------------------- | ------------------------------------ |
| Flanco | Cambio de señal     | Detecta transiciones rápidas         |
| Nivel  | Nivel sostenido LOW | Puede generar múltiples activaciones |


**g) Interrupciones desde los puertos de Entrada/Salida (PIN Change Interrupts).**
- **Describa que terminales I/O pueden generar interrupciones por cambio de estado**
  
  Todos los pines de los puertos **B**, **C** y **D** (excepto los usados por INT0/1 si están habilitados) pueden generar interrupciones por cambio de estado mediante PIN Change Interrupts.

- **Investigue como se configuran estas interrupciones en los puertos de Entrada/Salida**
  Se activan usando tres registros:
  - `PCICR`: Habilita interrupciones por puerto (PCIE0/1/2).
  - `PCMSKx` (x = 0,1,2): Habilita pines individuales dentro del puerto.
  - `PCIFR`: Banderas de interrupción por cambio de pin.
- **Describa cuales son los vectores de interrupción asociados y sus prioridades relativas.**
  Hay tres vectores:
  - `PCINT0_vect` → Puerto B
  - `PCINT1_vect` → Puerto C
  - `PCINT2_vect` → Puerto D  
  Su prioridad depende del orden en la tabla de vectores: PCINT0 > PCINT1 > PCINT2.
- **Comente las diferencias más importantes con las interrupciones del punto f).**
  Las **INTx** (INT0/1) tienen detección por flanco o nivel y son más precisas, pero limitadas a dos pines. Las **PCINT** permiten interrupción en muchos pines pero solo detectan cambio de estado (sin distinguir subida o bajada) y comparten el mismo vector por grupo.


## Ejercicio No 4: Temporizadores

**a) Describa los componentes principales del módulo TIMER0 y explique sus modos de funcionamiento.**

TIMER0 cuenta con los siguientes componentes:
- **Contador (TCNT0):** lleva el valor actual de la cuenta con 8 bits.
- **Registro de comparación (OCR0A/B):** permite comparar el valor del contador para generar eventos.
- **Prescaler:** divide la frecuencia del reloj para ajustar la velocidad del conteo.
- **Fuente de reloj:** puede ser interna o externa, seleccionada por bits en TCCR0B.
- **Registro de control (TCCR0A/B):** configura el modo de operación y el prescaler.
- **Registro de interrupciones (TIMSK0):** habilita interrupciones por comparación o desborde.
- **Banderas (TIFR0):** indican eventos como desbordes o coincidencias.

Modos de funcionamiento:
- **Modo Normal:** el contador cuenta de 0 a 255 y luego vuelve a 0. Se genera una interrupción por desborde (cuando TCNT0 pasa de 255 a 0).
- **Modo CTC (Clear Timer on Compare Match):** el contador cuenta hasta el valor de OCR0A y luego se reinicia a 0. Permite generar tiempos precisos y generar interrupciones al llegar al valor de comparación.
- **Modos PWM (Fast PWM y Phase Correct PWM):** generan señales de modulación por ancho de pulso (PWM), útiles para controlar brillo de LEDs o velocidad de motores.

**b) Explique cómo funciona el prescalador (Prescaler) y obtenga una expresión para calcular la frecuencia de reloj del contador (CLKTn) y la frecuencia de desborde (Overflow) en función de las distintas opciones de fuentes de reloj. ¿Cómo se borra la bandera de desborde?**

El prescaler divide la frecuencia del reloj del microcontrolador para reducir la velocidad del contador.

$$CLKTn = f_{CLKIO} / N, \text{donde N es el valor del prescaler (1, 8, 64, 256, 1024)}.$$
- En modo normal: 
  $$f_{overflow} = CLKTn / 256$$  

- En modo CTC: 
  $$f_{interrupción} = CLKTn / (OCR0A + 1)$$  

La bandera de desborde (TOV0) se borra escribiendo un 1 en su bit correspondiente del registro TIFR0.


**c) Si se tiene una frecuencia de CLKIO de 8MHz, establezca con que valores debería inicializar los registros pertinentes del TIMER0 para realizar un retardo de 1ms. ¿es posible generar un retardo de 1000 ms?**

Sea $f_{CLKIO} = 8MHz$, $N$ el valor del prescaler, se tiene

$$T \text{(periodo)} = N / f_{CLKIO}$$

Para 1ms: $\frac{1ms}{T} < 255$ => $\frac{1ms}{N / f_{CLKIO}} < 255$.

Como queremos el N lo más chico posible (para dividir lo menos posible la frec. original), tomo N=64, lo que nos da $OCR0A = \frac{1ms}{N / f_{CLKIO}} = 124$ en modo CTC.

Para 1000ms: $\frac{1ms}{T} < 255$ => $\frac{1000ms}{N / f_{CLKIO}} < 255$.

Como N=1, 8, 64, 256 o 1024, no hay ningún valor del prescaler que permita hacer el delay.


**d) Si se tiene una frecuencia de CLKIO de 8MHz, configure TIMER0 para generar una interrupción periódica de 1ms.**

```c
TCCR0A = (1 << WGM01); // Modo CTC
TCCR0B = (1 << CS01) | (1 << CS00); // N = 64
OCR0A = 124;  // Comparador con 124
TIMSK0 |= (1 << OCIE0A); // Habilita la interrupción al llegar al 124
```


## Ejercicio No 5: Temporizador Asincrónico–RTC (Contador de Tiempo Real)

**a) Analice las diferencias y similitudes entre TIMER0 y TIMER2.**

Ambos timers son de 8 bits con modos de operación normal, CTC y PWM. Ambos difieren en que el TIMER0 es síncrono con el reloj del sistema, mientras que el TIMER2 puede trabajar de forma asíncrona con una fuente de reloj externa.


**b) Si se usa el cristal externo de 32.768kHz, determine con que valores debería inicializar los registros pertinentes del módulo para obtener una interrupción periódica de 10 ms.**

Ni idea.


**c) Busque información sobre los circuitos integrados IC: DS1302 y DS3231 ¿Qué funcionalidad proveen? ¿Cómo se conectan a un MCU? ¿Qué ventajas le encuentra a su uso?**

- **DS1302**:
  - **Funcionalidad**: Es un **RTC (reloj en tiempo real)** con interfaz serial (I2C o SPI).
  - **Conexión a MCU**: Conecta a través de líneas **SDA**, **SCL** y **VCC/GND**.
  - **Ventaja**: Bajo consumo de energía, adecuado para sistemas portátiles.

- **DS3231**:
  - **Funcionalidad**: **RTC preciso** con compensación automática de temperatura.
  - **Conexión a MCU**: Se conecta a través de **I2C** (SDA/SCL).
  - **Ventaja**: Mayor precisión que el DS1302, con compensación automática de la temperatura para mayor exactitud a largo plazo. Ideal para aplicaciones donde la precisión es crucial.


## Ejercicio No 6: Implementación de Maquinas de Estado Finitos (MEF)

**a) ¿Qué es un modelo de Estados Finitos? ¿Qué elementos contiene? ¿Cómo se especifica?**

Un modelo de Estados Finitos (MEF) es un sistema que cambia de estado en función de entradas. Contiene:

- Conjunto finito de estados.
- Entradas.
- Salidas.
- Función de transición.
- Función de salida.

Se especifica mediante diagramas, tablas o pseudocódigo.


**b) ¿Cuál es la diferencia entre el modelo propuesto por Moore y el de Mealy?**

- Moore: la salida depende solo del estado actual.
- Mealy: la salida depende del estado actual y la entrada. Mealy suele reaccionar más rápido a las entradas.


**c) Describa diferentes maneras para implementar en C un modelo MEF, ¿Cómo conviene declarar las variables de estados? De un ejemplo resumido del código para una posible implementación.**

```c
typedef enum {ESTADO_INICIAL, ESTADO_ACTIVO, ESTADO_FINAL} Estado_t;
Estado_t estado = ESTADO_INICIAL;

void actualizarMEF(int entrada) {
  switch (estado) {
    case ESTADO_INICIAL:
      if (entrada) estado = ESTADO_ACTIVO;
      break;
    case ESTADO_ACTIVO:
      if (!entrada) estado = ESTADO_FINAL;
      break;
    case ESTADO_FINAL:
      // mantener o reiniciar
      break;
  }
}
```

**d) ¿Cómo se inicializa una MEF? ¿Cómo se actualiza una MEF?**

Se inicializa con un estado inicial. Se actualiza según las entradas, se evalúa y se cambia de estado.


**e) Analice diferencias y similitudes de una implementación MEF en lenguaje C y en lenguaje VHDL partiendo del mismo modelo de la MEF.**

Diferencias:
- VHDL es concurrente y orientado a hardware; C es secuencial.
- En VHDL se usan procesos y señales; en C, funciones y variables.
- En VHDL se modela el tiempo; en C, no directamente.


## Ejercicio No 7: Planificación Cooperativa de Tareas y Modularización

**a) Explique en qué consiste un esquema de planificación cooperativo con interrupción periódica de timer.**

Un esquema de planificación cooperativo implica que cada tarea debe ceder voluntariamente el control del CPU después de ejecutarse. No hay un cambio forzado de contexto como en la planificación por prioridad o preemptiva.

En un sistema cooperativo con interrupción periódica de timer, se utiliza un temporizador (Timer) del microcontrolador configurado para generar una interrupción a intervalos regulares (por ejemplo, cada 1 ms). En cada interrupción, se actualiza un "tick del sistema" y se decide si alguna tarea debe ejecutarse.


**b) Determine el periodo de la interrupción para la planificación de tres tareas periódicas x(), y(), z() con períodos 10, 25 y 50ms respectivamente. ¿Qué condiciones tiene que asumir para que el funcionamiento del sistema esté garantizado y sea confiable?**

Dado que las tareas tienen periodos de $T_{x} = 10ms, T_{y} = 25ms \text{ y } T_{z} = 50ms$, el periodo de la interrupción debe ser tal que permita evitar perder ticks de interrupción y evitar desincronizaciones. Por lo tanto, el periodo de la interrupción debe ser el MCD (Mínimo Común Divisor):

$$T_{\text{interrupcion}} = MCD(5ms, 25ms, 50ms) = 5ms$$

Para que funcione correctamente se debe cumplir:
- Ni idea.


**c) Explique mediante pseudocódigo como implementar el planificador y despachador de las tareas y muestre con un diagrama temporal un ejemplo de la ejecución de las mismas.**




**d) En el caso de que haya superposición de tareas en el mismo tick de sistema, explique cómo puede hacerse la planificación del inciso b) para que no haya superposición en la ejecución de x(), y(), z().**



**e) Explique qué entiende por modularización de software y qué ventajas tiene.**

Es la división de un programa en partes lógicas independientes llamadas módulos, que pueden ser desarrolladas, probadas y mantenidas por separado.

Ventajas:
- Reutilización de código.
- Mejor mantenimiento y legibilidad.
- Facilitación de pruebas unitarias.
- Permite trabajo en equipo más ordenado.
- Reducción del acoplamiento

**f) Explique la manera de modularizar archivos en C a partir del concepto de clases tomado de la P.O.O.**

Se pueden simular las clases de P.O.O tomando que cada clase es representada por un archivo ``.h``:

```c
// sensor.h
#ifndef SENSOR_H
#define SENSOR_H

void sensor_init(void);
int sensor_read(void);

#endif
```

```c
// sensor.c
#include "sensor.h"
#include <avr/io.h>

void sensor_init(void) {
    // Inicializa ADC, etc.
}

int sensor_read(void) {
    // Lee el valor analógico
    return ADC;
}
```


**g) Explique cómo se emplea el modificador static con las funciones, variables y constantes para modularizar un programa en C.**

Las variables static:
- **En funciones:** funciones privadas o internas del módulo no interfieran con otras funciones del mismo nombre en otros módulos.
- **En variables:** Dentro de funciones conservan su valor entre llamadas.


**h) Explique que son las funciones privadas y las funciones públicas de un módulo, de un ejemplo de implementación de cada una.**

- **Públicas:** Declaradas en el .h (header), accesibles desde otros módulos.
- **Privadas:** No están en el header, y usualmente se marcan como static en el .c.


## Ejercicio No 8: Sistema de generación y distribución de reloj

**a) Explique cuáles son las posibles fuentes de reloj y sus rangos de frecuencia para que funcione el MCU.**



**b) Explique cómo se distribuyen internamente las señales de reloj para la CPU y los distintos periféricos según las diferentes opciones (diagrama en bloques de la distribución interna de las señales de reloj).**


**c) Investigue sobre cómo utilizar la biblioteca de funciones de bajo consumo (sleep.h).**

La biblioteca sleep.h permite poner al microcontrolador en modos de bajo consumo, reduciendo consumo de energía cuando no se necesita que esté procesando activamente.


**d) Explique cuál es la configuración del reloj del sistema dispuesta desde fábrica y que tolerancia tiene la frecuencia del mismo.**


# EJERCICIOS PARA REALIZAR CON SIMULADOR

## Ejercicio No 11: MEF

**a) Una forma de implementar un modelo MEF en software es mediante la sentencia switch-case. Utilice como ejemplo el diagrama de estados del controlador de un semáforo y explique este método describiendo como implementar las variables de estados, las entradas, las salidas y las transiciones**

Dado que la transición de un estado a otro no depende de ninguna entrada en un semáforo, se utiliza una MEF de Moore. La forma de representarlo mediante un diagrama de estados es el siguiente:

![MEF_Semaforo](/Practicas/Practica2/semaforo.png)

Para implementarlo en código puede utilizarse una función que sirva de inicializar de la MEF, se guarda el estado y salida iniciales, los cuales estan definidos por enums. Luego, se itera y se actualiza utilizando un switch-case donde cada case representa el estado y dentro del mismo, se implementa la lógica para obtener la salida y la transición al nuevo estado (de requerirse).
