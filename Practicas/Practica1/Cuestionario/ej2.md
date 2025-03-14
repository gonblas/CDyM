# Ejercicio No 2: Familia de microntroladores AVR

**a) Investigue sobre los diferentes modelos de la familia Atmega AVR. Tabule los periféricos, la cantidad de RAM y de FLASH que poseen las distintas versiones. En particular detalle las características de los modelos Atmega328P y Atmega2560 utilizados en las plataformas Open-Source Arduino UNO y MEGA.**

|    Name    | Architecture | Pins  |     Flash     | SRAM  |
| :--------: | :----------: | :---: | :-----------: | :---: |
| Atmega328P |     AVR      |  28   |     32KB      |  2KB  |
| Atmega2560 |     AVR      |  100  | 64K/128K/256K |  8KB  |

Información: [Atmega328P](https://ww1.microchip.com/downloads/en/DeviceDoc/Atmel-7810-Automotive-Microcontrollers-ATmega328P_Datasheet.pdf) y [Atmega2560](https://ww1.microchip.com/downloads/en/devicedoc/atmel-2549-8-bit-avr-microcontroller-atmega640-1280-1281-2560-2561_datasheet.pdf).



**b) Detalle las características de la CPU AVR, incluyendo: Arquitectura de la CPU (Realice un diagrama en bloques), modelo de programación, modos de direccionamiento y resumen del conjunto de instrucciones.**


|      Caracteristica       |                                                           Dato                                                            |
| :-----------------------: | :-----------------------------------------------------------------------------------------------------------------------: |
|       Arquitectura        |                                             Harvard (CPU Basada en Registros)                                             |
|   Set de instrucciones    |                                                 RISC (+130 instrucciones)                                                 |
|  Modelo de programación   |                                                    Pipeline (2-etapas)                                                    |
| Modos de direccionamiento | 5: Directo, indirecto, indirecto con desplazamiento, indirecto con pre-desplazamiento e indirecto con post-desplazamiento |

[Conjunto extendido](https://ww1.microchip.com/downloads/en/devicedoc/AVR-Instruction-Set-Manual-DS40002198A.pdf)



**c) Puertos de Entrada/Salida: Describa qué registros se utilizan para controlar los puertos de entrada y salida. Realice un diagrama en bloques de un terminal de entrada y salida y explique su funcionamiento.**

Los microcontroladores AVR utilizan los puertos PORTA, PORTB, PORTC, PORTD para interactuar con dispositivos externos, cada uno de los puertos consiste en 8 pins que pueden ser configurados como entrada o salida usando los registros DDRx, PORTx y PINx:

- **DDRx (Data Direction Register):** configuran los pins como entrada (0) o salida (1).
- **PORTx (Output port register):** establece los valores de salida para los pins de salida y habilita/deshabilita los resistores pull-up en los pins de entrada.
  > Cuando un pin está configurado como entrada (DDRx = 0), su estado es indefinido si no está conectado a nada. Para evitar esto, se puede activar una resistencia pull-up interna (PORTx = 1), que lo mantiene en alto (1, Vcc) cuando no hay una señal externa. Si conectas un botón a un pin configurado como entrada, al presionar el botón se conectará a GND (0, bajo). Si no lo presionas, la resistencia pull-up lo mantiene en Vcc (1, alto), evitando valores erráticos.
- **PINx (Input Register):** utilizados para leer la logica de los pins de los puertos. Basicamente, ver si un pin esta en alto o bajo. 


**d) Investigue cual es la capacidad de corriente que puede manejar un terminal individual configurado como salida. ¿Depende del estado lógico? ¿cuál es la capacidad de corriente que puede manejar el microcontrolador con todos los puertos funcionando simultáneamente?**

La capacidad de corriente que puede manejar un terminal individual configurado como salida es 40mA. La máxima que puede manejar el microcontrolador con todos sus puertos funcionando simultáneamente es de 200mA.


**e) Indique cuales son las dos posibilidades de conexión de un LED a un terminal de entrada y salida. Calcule la resistencia serie para que la corriente máxima por cada LED sea de 5mA. Muestre como configurar el terminal como salida y como modificar su estado lógico.**


Las dos formas de conexión de un LED a un terminal de entrada y salida son:
- **Sinking:** El LED se conecta con su ánodo (+) a VCC y su cátodo (-) a una resistencia en serie, que luego va al pin del microcontrolador. Para encender el LED, el pin debe ponerse en 0 lógico (LOW), permitiendo que la corriente fluya desde VCC hacia GND a través del LED.

- **Sourcing:** El LED se conecta con su cátodo (-) a GND y su ánodo (+) a una resistencia en serie, que luego va al pin del microcontrolador. Para encender el LED, el pin debe ponerse en 1 lógico (HIGH), permitiendo que la corriente fluya desde el pin hacia GND a través del LED.

$$
  R = \frac{V}{I_{max}} = \frac{V_{cc} - V_{LED}}{I_{max}} =  \frac{5V - 2.2V}{5mA} = 560 ~\Omega \quad\quad\text{para LED Amarillo}
$$


Usando el puerto C:
```c
  DDRC |= (1 << PORTC0); // salida
  PORTC |= (1 << PORTC0); // modificar estado lógico
```


**f) Se desea conectar un pulsador a una entrada digital del MCU. Investigue los posibles esquemas de conectar un pulsador y determine el algoritmo más adecuado para detectar en cada caso cuando el pulsador se presiona y se suelta. ¿Qué es una resistencia de pull-up? ¿y de pull-down? ¿Importa su valor? Investigue sobre cómo utilizar los pull-up internos del MCU.**

Las dos posibles formas de conectar un pulsador a una entrada digital del MCU son:
- **Con resistencia pull-up:** Se conecta un extremo del pulsador a GND y el otro a la entrada digital, con una resistencia pull-up conectada a VCC. Cuando el pulsador no se presiona, la entrada está en 1 lógico (HIGH). Al presionar, se conecta a GND y la entrada cambia a 0 lógico (LOW).
- **Con resistencia pull-down:** Se conecta un extremo del pulsador a VCC y el otro a la entrada digital, con una resistencia pull-down conectada a GND. Cuando el pulsador no se presiona, la entrada está en 0 lógico (LOW). Al presionar, se conecta a VCC y la entrada cambia a 1 lógico (HIGH).

El algoritmo para detectar el estado del pulsador se lee el estado del pin cada cierto tiempo.

Una resistencia de pull-up es una resistencia utilizada en circuitos de lógica digital para asegurarse que una entrada se encontrada en 1, para evitar estados indefinidos. Las de pull-down es idem pull-up pero para el estado 0. Para utilizar los pull-up se debe setear como entrada un terminal de uno de los puertos, poniendo luego un 1 con PORTx en el terminal correspondiente.
