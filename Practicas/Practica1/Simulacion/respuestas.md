# EJERCICIO DE SIMULACIÓN:

El objetivo principal del siguiente ejercicio es abordar la programación en C de dispositivos AVR, comenzando por programas simples que utilizan los puertos de entrada-salida, y el uso del conjunto de herramientas de desarrollo para abarcar el proceso completo de edición, compilación, simulación del dispositivo, simulación de  la aplicación o la placa de entrenamiento y finalmente la programación del dispositivo real.


**a) Analice el programa ejemplo de la Fig.1 y comprenda su funcionamiento línea a línea.**

El programa de la figura 1 configura uno de los terminales del puerto C como entrada, con una resistencia pull-up para que el estado por defecto sea en alto y el puerto D como salida. Entonces, si hay un '1' en el terminal correspondiente al bit0 del puerto C se obtiene como salida un patron de bits en el puerto D, sino se obtiene el contrario. Luego, limpia la salida del puerto D.


**b) Explique cómo funciona la función _delay_ms() de la bibliotecas de AVR Libc. [AVR Libc Reference Manual](https://onlinedocs.microchip.com/oxy/GUID-317042D4-BCCE-4065-BB05-AC4312DBC2C4-en-US-2/index.html)**

La función ``_delay_ms(number)`` realiza un delay de ``number`` dependiendo del valor asignado en la macro ``F_CPU`` en Hertz con el delay maximo posible de $262.14~ms/\text{F\_CPU in MHz}$.


[Información](https://onlinedocs.microchip.com/oxy/GUID-317042D4-BCCE-4065-BB05-AC4312DBC2C4-en-US-2/GUID-8F9E773B-F166-41A9-B106-F5BA34DFE2FE_2.html).



**c) Cree un proyecto nuevo en Microchip Studio 7 y copie el programa del ejemplo en el editor, guarde el archivo “.c” y compile.**

Listo.


**d) Analice el resultado de la compilación: errores, advertencias, memoria de datos y de programa utilizada.**

- **Errores:** 0.
- **Warnings:** 1. Memory usage estimation may not be accurate if there are sections other than .text sections in ELF file.
- **Memoria de datos:** 0 bytes.
- **Memoria de programa:** 240 bytes.


**e) Analice las dependencias externas: examine y comprenda la función que cumple la línea ``#include <avr/io.h>``.**

- ``#include <avr/io.h>``: permite acceder a los registros de control de los puertos.
- ``#include <util/delay.h>``: Para hacer delays.

**f) Simule el programa paso por paso con el simulador integrado Microchip Studio 7. Examine las ventanas de registros del MCU, de ciclos de reloj consumidos y el comportamiento de los puertos I/O utilizados.**

Listo.


**g) Realice en Proteus ISIS, un circuito que incluya el MCU, ocho pulsadores y ocho leds para simular el programa desarrollado. Simule y verifique el comportamiento del mismo. Utilice el archivo “.elf” para depurar el programa paso a paso.**

Listo.



___

**Código utilizado (Fig. 1):**

```C
/* Inclusión de bibliotecas de código */
#include <avr/io.h> // Registros del microcontrolador
#define F_CPU 16000000UL // Defino la frecuencia de oscilador en 8MHz
#include <util/delay.h> // Retardos por software
/* Función main */
int main (void)
{
  /* Setup */
  DDRD = 0xFF; // Configuro Puerto D como salida
  DDRC &= ~(1<<PORTC0); // Configuro bit0 puerto C como entrada
  PORTC |= (1<<PORTC0); // Habilito Pull-Up en bit0 puerto C
  /* Loop */
  while(1)
  {
    if (PINC & (1<<PINC0))
    {
      PORTD = 0b10101010; // Escribo Port D con patrón de bits
      _delay_ms(100); // Delay de 100 ms
      PORTD = 0x00; // Escribo Port D con todos 0
      _delay_ms(100); // Delay de 100 ms
    }
    else
    {
      PORTD = 0b01010101; // Escribo Port D con otro patrón de bits
      _delay_ms(100); // Delay de 100 ms
      PORTD = 0x00; // Escribo Port D con todos 0
      _delay_ms(100); // Delay de 100 ms
    }
  }
  /* Punto de finalización del programa (NO se debe llegar a este lugar) */
  return 0;
}
```