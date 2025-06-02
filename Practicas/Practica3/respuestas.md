# CUESTIONARIO

## Ejercicio No 1: Periférico UART

**a) Describa las características principales de una comunicación serie asincrónica.**

La transmision serie se realiza por un mismo canal de comunicacion (Tx) por el cual se envian varios paquetes, un bit a la vez. Algunas de las caracteristicas mas importantes de la comunicacion en serie asicronica son:

- **Tiempo de Bit (Tb):** es el tiempo de duración de un bit
- **Tasa de Transferencia (1/Tb):** es el número de bits por unidad de tiempo [bps]. También se la denomina baud-rate (tasa de símbolos), son iguales salvo que un simbolo representa mas de un bit.
- **Overhead:** son bits o bytes que se “agregan” al dato para hacer más confiable una transmisión. Por ejemplo: bit de paridad o bytes de checksum.
- **Bandwidth o Throughput:** es el número total de bits de información por unidad de tiempo, sin tener en cuenta el Overhead.


**b) Explique cómo funcionan los tres subsistemas principales del módulo UART del Atmega: el generador de tasa de transmisión (baud rate), el transmisor (Tx) y el receptor (Rx). Explique la funcionalidad de los registros correspondientes.**


El UART es el periférico integrado del microcontrolador que permite la transmisión de datos de forma asíncrona con otros dispositivos. En realidad, se trata de un USART (Universal Synchronous and Asynchronous Receiver/Transmitter), ya que también puede operar en modo síncrono si se desea aumentar la velocidad de transmisión. A continuación, se describen sus tres subsistemas principales y cómo interactúan durante una comunicación:

- **El generador de tasa de transmisión:** Todo comienza con la configuración de la velocidad a la que se quiere transmitir. El microcontrolador toma la frecuencia de su oscilador interno (16 MHz) y la divide mediante el *baud rate generator*, que utiliza los registros UBRRH y UBRRL para definir el divisor. Esto genera una frecuencia adecuada para la transmisión, como 9600, 19200 o 115200 baudios. Además, el registro *sync logic* permite ajustar el reloj para comunicaciones síncronas, y el bit U2X del registro UCSRA puede habilitar el doble de velocidad en modo asíncrono. El *pin control*, por su parte, habilita o deshabilita los pines físicos TX y RX según el modo de operación seleccionado (simplex, half-duplex y full-duplex).
- **El transmisor (Tx):** Una vez configurada la tasa de transmisión, el transmisor queda listo para enviar datos. Cuando se escribe un byte en el registro *UDR*, este se carga en un buffer y luego pasa al *transmit shift register*, donde se convierte a formato serial. El *Tx control* arma automáticamente la trama: primero genera un bit de START (generado por el *pin control*), luego los bits de datos, y si está habilitada la paridad (generada por el *parity generator*), se calcula y agrega el bit correspondiente. Finalmente, se añaden uno o dos bits de STOP (generado por el *pin control*).
- **El receptor (Rx):** El receptor está siempre atento al pin RX, controlado por el *pin control*, esperando detectar una bajada de tensión que indique el bit de START. Una vez detectado, el Rx control activa toda la lógica de recepción.

  El *clock recovery* ajusta internamente el reloj del receptor para sincronizarse con la señal entrante. Luego, el *data recovery* realiza varios muestreos dentro de cada bit recibido para decidir su valor lógico (utilizando el clock ajustado). Los bits válidos se van cargando uno a uno en el *receive shift register*, y si la verificación de paridad está habilitada (mediante los bits UPM1:0 en UCSRC), el *parity checker* compara el bit de paridad recibido con el calculado para detectar errores.

  Cuando el byte completo se ha recibido correctamente, se transfiere automáticamente al registro *UDR (Receive)*, listo para ser leído por el programa.


El transmisor y el receptor son independientes entre si, se puede usar el uno y no el otro, o ambos en simultáneo.


**c) Si se tiene una frecuencia de reloj de 16MHz, encuentre el valor de configuración para obtener tasas de transferencia de 1200, 2400, 4800, 9600, 19200 y 38400 bps. Determine el error resultante (en %) cometido en la aproximación respecto a los valores ideales.**


El baud rate esta dado por la siguiente formula:

$$\text{Baud rate} = \frac{F_{osc}}{(UBRR+1)\cdot 16}$$

Puesto que la frecuencia de reloj es de $f_{osc} = 16MHz$. El error asociado en porcentaje es:

$$E = \frac{\text{Baud rate}_{\text{ideal}} - \text{Baud rate}_{\text{real}}}{\text{Baud rate}_{\text{ideal}}} \cdot 100$$


Para las configuraciones requeridas:

$$
  \text{Baud rate} = \frac{F_{osc}}{(UBRR+1)\cdot 16} = 1200 \rightarrow UBRR = 832 ~~\therefore~~ E = 0.04\% \\
  \text{Baud rate} = \frac{F_{osc}}{(UBRR+1)\cdot 16} = 2400 \rightarrow UBRR = 415 ~~\therefore~~ E = 0.16\% \\
  \text{Baud rate} = \frac{F_{osc}}{(UBRR+1)\cdot 16} = 4800 \rightarrow UBRR = 207 ~~\therefore~~ E = 0.16\% \\
  \text{Baud rate} = \frac{F_{osc}}{(UBRR+1)\cdot 16} = 9600 \rightarrow UBRR = 103 ~~\therefore~~ E = 0.16\% \\
  \text{Baud rate} = \frac{F_{osc}}{(UBRR+1)\cdot 16} = 19200 \rightarrow UBRR = 51 ~~\therefore~~ E = 0.16\% \\
  \text{Baud rate} = \frac{F_{osc}}{(UBRR+1)\cdot 16} = 38400 \rightarrow UBRR = 25 ~~\therefore~~ E = 0.16\%
$$


**d) ¿Cuál es el error máximo en el baud rate que puede aceptarse en una comunicación serie asincrónica con formato 8N1 (8 bits de datos, sin paridad, 1 bit stop) para que la comunicación sea confiable?**

En una comunicación UART asincrónica con formato 8N1 (8 bits de datos, sin paridad, 1 bit de stop), se transmite un total de 10 bits por trama: 1 bit de start, 8 de datos y 1 de stop. Como no hay reloj compartido, el receptor debe muestrear cada bit basándose en su propio reloj, y generalmente lo hace en el centro del bit. Esto implica que el error de sincronización acumulado no puede superar medio bit de tiempo al final de la trama, o el receptor podría muestrear fuera del bit correcto. 

$$
  \frac{0.5 bits}{10 bits} = 0.05 = 5\%
$$

La diferencia de baud rate debe ser menor al $5\%$. Como solución a ello, el pin de entrada se muestrea a una tasa 16 veces mayor a la requerida (16 veces) de forma de detectar el bit de start y sincronizarse con el centro de los datos de la siguiente forma: tiene 3 bits (el 8, 9 y 10 de las 16 que toma) y se fija si los tres (que conforman el centro del bit muestreado) son iguales, en tal caso no hay problemas de ruido, en caso de que uno de ellos cambia el valor puede asumir que existe un problema de ruido.


## Ejercicio No 2: Periférico TIMER 1


**a) Describa el funcionamiento y los registros del módulo TIMER 1. Realice un diagrama en bloques del mismo.**

El **TIMER 1** es un periférico de tipo **temporizador/contador de 16 bits** presente en microcontroladores AVR como los ATmega. Su funcionamiento se basa en incrementar o decrementar un contador en función de una fuente de reloj interna o externa. Es versátil y permite generación de señales PWM, medición de señales externas, generación de interrupciones periódicas y comparación de tiempos.

Registros principales (comparables con los del TIMER 0):

| Registro       | Función                                                       |
|----------------|---------------------------------------------------------------|
| **TCNT1**      | Registro principal del contador (Timer/Counter 1)             |
| **OCR1A/B**    | Registros de comparación A y B (Output Compare Register)      |
| **TCCR1A/B**   | Registros de control (Timer/Counter Control Register A y B)   |
| **ICR1**       | Registro de captura de entrada (Input Capture Register)       |
| **TIMSK1**     | Registro de enmascaramiento de interrupciones                 |
| **TIFR1**      | Registro de banderas de interrupción                          |

> El TIMER 1 posee dos salidas de comparación (OC1A y OC1B), un sistema de captura de entrada (ICU), y múltiples modos de operación avanzados.




**b) Describa los modos de funcionamiento “normal” y “CTC”.**


Modo normal:

- El contador **TCNT1** se incrementa desde 0 hasta 0xFFFF (65535).
- Al alcanzar el valor máximo, **desborda** (overflow) y se genera la interrupción **TOV1** si está habilitada.
- El registro **OCR1A/B** puede usarse para comparación, pero no afecta el flujo del contador.
- No hay reinicio automático, solo al llegar al máximo.

**Uso típico:** medición de tiempo sin reinicio automático.

Modo CTC:

- El contador **TCNT1** se incrementa hasta que iguala **OCR1A** (o **ICR1**, según configuración).
- Cuando **TCNT1 == OCR1A**, se genera una **interrupción de comparación** (**OCF1A**), y el contador se reinicia automáticamente a 0.
- Esto permite generar señales periódicas precisas.

**Uso típico:** generación de señales periódicas, temporización precisa.




**c) Explique cómo funciona el subsistema “Waveform Generator”. ¿Cuántas unidades hay?**

El subsistema **Waveform Generator** (Generador de formas de onda) es responsable de generar señales de salida a partir del valor del contador del timer y los registros de comparación.

Este módulo interpreta el valor actual del contador (**TCNT1**) en relación con los registros de comparación (**OCR1A**, **OCR1B**) y con el modo de operación configurado en los registros **TCCR1A** y **TCCR1B** respectivamente, para definir la señal que se emite por los pines de salida (**OC1A** y **OC1B**).

**Funcionalidades principales:**

- Comparar el valor del contador con los registros OCRnA/B.
- Generar salidas PWM (Fast PWM, Phase Correct PWM, Phase and Frequency Correct PWM).
- Generar señales cuadradas o pulsos en modo CTC.
- Actualizar los pines de salida OC1A y OC1B automáticamente según la comparación.

**¿Cuántas unidades hay?**

Existen **dos unidades independientes de generación de forma de onda**:

- Una para el canal **A**: asociada al pin **OC1A**.
- Otra para el canal **B**: asociada al pin **OC1B**.

Cada una de estas unidades puede configurarse de forma distinta y trabajar con su propio registro de comparación (**OCR1A** u **OCR1B**), lo que permite generar dos señales PWM distintas y simultáneas desde un mismo timer.





**d) Describa un método para generar una señal digital de frecuencia o periodo programable. Analice la resolución y el rango obtenido en función de los parámetros de configuración.**


Una forma común de generar una señal digital de frecuencia o período programable con el TIMER 1 es utilizar el modo CTC (Clear Timer on Compare Match).

El procedimiento consiste en:

- Configurar TIMER 1 en modo CTC (WGM1[3:0] = 0b0100 o 0b1100).
- Seleccionar el prescaler mediante los bits CS1[2:0] en el registro TCCR1B.
- Cargar el valor deseado en el registro OCR1A.
- Activar la salida OC1A (toggle automático) o usar la interrupción por comparación (OCF1A).

La frecuencia de salida se calcula con la fórmula:

\[
f_{\text{out}} = \frac{f_{\text{clk}}}{2 \cdot \text{prescaler} \cdot (1 + OCR1A)}
\]

La resolución está dada por los 16 bits del registro OCR1A, permitiendo valores de 0 a 65535.

El rango de frecuencias generables depende del valor del prescaler y OCR1A:

- Frecuencia máxima: con prescaler = 1 y OCR1A = 0, se obtiene f_out ≈ 8 MHz.
- Frecuencia mínima: con prescaler = 1024 y OCR1A = 65535, se obtiene f_out ≈ 0.12 Hz.

Este método permite generar señales digitales de frecuencia programable con alta resolución y amplio rango de ajuste.




**e) Explique el funcionamiento del subsistema “Input Capture”. ¿Cuántas unidades hay?**


El subsistema Input Capture permite registrar el valor del contador (TCNT1) en el momento exacto en que se detecta un evento en el pin ICP1 (por defecto, el pin PD6 en algunos microcontroladores AVR). Este evento puede ser un flanco ascendente o descendente configurable. Al ocurrir el flanco, el valor de TCNT1 se copia automáticamente en el registro ICR1 (Input Capture Register 1), y se puede generar una interrupción si está habilitada. Este mecanismo es muy útil para medir tiempos sin intervención del software en el instante crítico del evento. El TIMER 1 posee una sola unidad de captura de entrada, por lo tanto hay una única unidad Input Capture.



**f) ¿Cómo lo utilizaría para medir el ancho de pulso de una señal externa? ¿Cómo lo utilizaría para medir la frecuencia o período de una señal periódica? De un ejemplo de uso. En ambos casos, defina y analice la resolución y el rango de medida.**

Para medir el ancho de pulso de una señal externa, se configura el Input Capture para detectar flancos alternados. Primero se configura para capturar el flanco ascendente y se guarda el valor de ICR1 cuando ocurre. Luego, se cambia la configuración para capturar el flanco descendente, y se guarda el nuevo valor de ICR1. La diferencia entre ambos valores representa el ancho del pulso en ticks del timer. Multiplicando esa diferencia por el tiempo de un tick (determinado por el prescaler y la frecuencia del reloj), se obtiene el tiempo en segundos. Para medir el período o la frecuencia de una señal periódica, se capturan dos flancos del mismo tipo (por ejemplo, dos flancos ascendentes). La diferencia entre los dos valores de ICR1 da el período de la señal, y su inverso es la frecuencia. 

Un ejemplo de uso sería medir la señal de salida de un sensor ultrasónico o de un sensor de caudal. La resolución depende del tiempo de un tick del timer, que se calcula como: tiempo_tick = prescaler / f_clk. El rango de medida depende del valor máximo que puede alcanzar TCNT1 (65535 para 16 bits) y del prescaler utilizado. Por ejemplo, con un reloj de 16 MHz y un prescaler de 64, el tiempo de un tick es 4 µs, y el rango máximo de medición sin desbordar el timer es 65535 × 4 µs ≈ 262 ms.



**g) El sensor de temperatura MAX6577 convierte la temperatura ambiente en una señal digital cuya frecuencia es proporcional a la temperatura en °C. Investigue la hoja de datos de este dispositivo y diga si es posible utilizar el TIMER 1 para medir la temperatura de un ambiente con dicho sensor y el MCU.**


Según la hoja de datos del sensor MAX6577, la frecuencia de salida varía entre 35 kHz y 100 kHz en función de la temperatura. Esta frecuencia es proporcional a la temperatura en °C según una constante definida por el fabricante. Dado que se trata de una señal digital periódica, es totalmente posible utilizar el TIMER 1 en modo Input Capture para medir su período o su frecuencia. Midiendo el período entre dos flancos del mismo tipo (por ejemplo, flancos ascendentes) y aplicando la fórmula inversa de conversión, se puede calcular la temperatura. Se recomienda usar un prescaler pequeño para obtener buena resolución temporal. El TIMER 1, por su capacidad de 16 bits y su precisión, es una excelente opción para esta aplicación.





## Ejercicio No 3: Background / Foreground – Drivers de periféricos


**a) Explique las características más importantes de la arquitectura de software Background / Foreground.**


La arquitectura Background / Foreground se basa en dividir el software en dos partes bien definidas:

- **Foreground (primer plano):** Es la parte que responde rápidamente a eventos, generalmente manejada mediante interrupciones. Aquí se hacen tareas cortas y críticas, como leer datos de un periférico o cambiar el estado de una variable para indicar que algo pasó.
- **Background (fondo):** Es un ciclo infinito donde se ejecutan tareas menos urgentes o que requieren más tiempo, como procesar los datos recibidos o actualizar estados internos del sistema.

Esta división ayuda a que el sistema sea eficiente sin necesidad de un sistema operativo complejo, evitando esperas activas y bloqueos. No hay multitarea real, sino que el foreground interrumpe al background cuando ocurre un evento que debe atenderse inmediatamente. La comunicación entre ambos contextos se hace normalmente a través de variables compartidas que deben manejarse con cuidado para evitar problemas.



**b) Describa el modelo de tarea cuando se ejecuta en dicho contexto.**

En este modelo, las tareas se dividen así:

- **En el foreground:** Se ejecutan rutinas breves e inmediatas que responden a interrupciones. Por ejemplo, una interrupción por recepción de dato en UART que almacena ese dato en un buffer.
- **En el background:** Se ejecutan las tareas principales del programa que revisan periódicamente el estado de esas variables o buffers y realizan el procesamiento necesario (como interpretar datos o enviar respuestas).

Así, el foreground se encarga de la captura o el envío rápido de datos, y el background hace el trabajo pesado sin interrumpir la respuesta rápida del sistema.






**c) Compare la arquitectura Background / Foreground con la utilizada en el TP2 que solo disponía de una única interrupción periódica. Exponga ventajas y desventajas.**


**Ventajas de Background / Foreground:**

- Permite manejar múltiples eventos de forma rápida y eficiente mediante interrupciones específicas.
- El foreground atiende solo lo urgente y el background puede dedicarse a tareas más complejas sin bloquear el sistema.
- Mejora la capacidad de respuesta frente a eventos asíncronos.

**Desventajas:**

- Requiere cuidado para manejar la comunicación entre foreground y background, evitando condiciones de carrera.
- Más complejo de diseñar.

**En cambio, la arquitectura del TP2 con una única interrupción periódica:**

- Es más simple porque solo hay una interrupción que se ejecuta periódicamente.
- Las tareas se hacen dentro del ciclo principal y en la rutina de interrupción periódica.
- Puede generar retardos en la atención a eventos críticos porque todo se revisa en bloques periódicos.
- Menor capacidad para manejar eventos asíncronos rápidamente.




**d) Realice una implementación en pseudocódigo de un driver para el periférico UART aplicando el modelo productor-consumidor.**

```pseudocode
// Buffer circular para recepción y transmisión
bufferRx = nuevoBufferCircular(tamaño)
bufferTx = nuevoBufferCircular(tamaño)

// Interrupción de recepción UART (Foreground)
ISR_UART_Rx():
    dato = leerDatoUART()
    if bufferRx no está lleno:
        bufferRx.escribir(dato)
    else:
        // Manejar buffer lleno (ejemplo: descartar dato o error)

// Interrupción de transmisión UART (Foreground)
ISR_UART_Tx():
    if bufferTx no está vacío:
        dato = bufferTx.leer()
        enviarDatoUART(dato)
    else:
        deshabilitarInterrupcionTx()

// Función para enviar dato (Background)
func enviarDato(dato):
    while bufferTx está lleno:
        esperar()  // Espera activa o pasa el control al background
    bufferTx.escribir(dato)
    habilitarInterrupcionTx()

// Función para leer dato (Background)
func leerDato():
    if bufferRx no está vacío:
        return bufferRx.leer()
    else:
        return null  // No hay datos disponibles

// Ciclo principal (Background)
while true:
    dato = leerDato()
    if dato != null:
        procesarDato(dato)
    // Otras tareas de fondo
```






## Ejercicio No 4: Periférico SPI-I2C


**a) Describa cuales son las características principales de una comunicación serie sincrónica SPI. Muestre como interconectar el MCU con varios dispositivos esclavos genéricos.**

SPI (Serial Peripheral Interface) es un protocolo de comunicación serie y sincrónico, lo que significa que los datos se transfieren bit a bit en sincronía con una señal de reloj. Es full dúplex, así que puede enviar y recibir datos al mismo tiempo. La velocidad puede llegar hasta los 10 Mbps o más, dependiendo del microcontrolador y del periférico.

Una conexión SPI típica involucra 4 líneas:

- MOSI (Master Out Slave In): datos que el maestro envía al esclavo.
- MISO (Master In Slave Out): datos que el esclavo devuelve al maestro.
- SCK (Serial Clock): señal de reloj generada por el maestro.
- SS (Slave Select): línea de selección del esclavo (activa en bajo).


**b) Explique cómo funciona el módulo SPI del Atmega. Describa la funcionalidad de los registros correspondientes.**

El Atmega tiene un módulo SPI por hardware que facilita toda la comunicación. Los registros más importantes son:

- SPCR (SPI Control Register): se usa para habilitar el SPI, elegir si el MCU es maestro o esclavo, configurar la polaridad y fase del reloj, y habilitar interrupciones si se desean.

- SPSR (SPI Status Register): muestra el estado del SPI, como si ya terminó una transmisión (flag SPIF) o si se está duplicando la velocidad.

- SPDR (SPI Data Register): es el registro donde se escribe el dato a enviar y también donde se lee el dato recibido.

Cuando se escribe un dato en SPDR, el módulo automáticamente empieza a transmitir y a recibir al mismo tiempo. Cuando termina, el bit SPIF de SPSR se pone en 1.



**c) Analice la hoja de datos del DS1302 y explique cómo conectarlo, es decir cual terminal del MCU corresponde conectar con cual terminal del dispositivo externo.**

El DS1302 es un reloj de tiempo real (RTC) que utiliza un protocolo de comunicación similar a SPI, pero con algunas diferencias. Tiene tres líneas:

- **CE** (Chip Enable): habilita la comunicación (similar a SS).
- **SCLK**: señal de reloj.
- **I/O**: línea bidireccional de datos (a diferencia del SPI, que tiene MOSI y MISO separados).


Conexion al MCU:

- DS1302 → MCU (pines configurados como GPIO)
- CE (pin 1) → Pin digital como salida (CS)
- SCLK (pin 2) → Pin digital como salida (CLK)
- I/O (pin 3) ↔ Pin digital como entrada/salida (Datos)

Es importante configurar el pin de datos como entrada o salida según el momento del protocolo, ya que I/O es una línea única bidireccional.





**d) Explique cuáles son las características principales de una comunicación serie sincrónica I2C y como se utiliza para conectar múltiples dispositivos. Analice ventajas y desventajas respecto al protocolo SPI.**

I2C (Inter-Integrated Circuit) es otro protocolo serie sincrónico, pero se diferencia de SPI en varios aspectos:

- **Solo usa 2 líneas**:
  - **SDA** (Serial Data): línea de datos bidireccional.
  - **SCL** (Serial Clock): línea de reloj.
- **Multimaestro y multiesclavo**: varios dispositivos pueden compartir el bus.
- **Cada dispositivo tiene una dirección única**: el maestro inicia la comunicación indicando con qué esclavo desea hablar.
- **Half dúplex**: los datos solo se envían o reciben en un sentido a la vez.

Ventajas frente a SPI:
- Menor cantidad de pines (solo 2 vs 4).
- Facilidad para conectar muchos dispositivos sin agregar más líneas.

Desventajas frente a SPI:
- Velocidad menor (generalmente hasta 400 kHz, aunque existen modos más rápidos).
- Más complejo de implementar (manejo de bits ACK/NACK, start/stop, etc.).
- Al compartir el bus, puede haber más problemas de colisión si no se gestiona bien.



**e) Analice la hoja de datos del DS3231 y explique cómo conectarlo a la interfaz TWI (I2C) del microntrolador Atmega utilizado en clase.**

El DS3231 es un reloj de tiempo real de alta precisión que se comunica por I2C. Sus pines relevantes son:

- **SDA (pin 5)**: línea de datos.
- **SCL (pin 6)**: línea de reloj.
- **VCC, GND**: alimentación.
- Opcionalmente: **SQW/INT** para generar interrupciones periódicas.

Conexión al microcontrolador Atmega (por ejemplo, ATmega328P):

DS3231 → MCU (Atmega)

SDA (pin 5) → PC4 (pin 27) - SDA (TWI Data)
SCL (pin 6) → PC5 (pin 28) - SCL (TWI Clock)
VCC → 5V
GND → GND


- Es necesario colocar resistencias pull-up (~4.7 kΩ) en las líneas SDA y SCL.
- El Atmega debe tener habilitado el módulo TWI (Two-Wire Interface), que es su implementación de I2C por hardware.
- Se configura la dirección del DS3231 (normalmente 0x68) en el maestro para iniciar la comunicación.



