**e) Uso del compilador Microchip (\*\*). Explique qué hacen las siguientes sentencias de C: DDRC=0x0F;PORTC=0x0C;**

La sentencia ``DDRC=0x0F`` configura los 4 terminales menos significativos como salida, y los 4 más significativos como entrada del puerto C. La sentencia ``PORTC=0x0C`` establece los valores de salida de los puertos de salida poniendo poniendo al b3 y b2 en 1, y al resto es 0 pues 0xC = 0b1100. Además, deshabilita las resistencias pull-up para los terminales de entrada.

**f) Explique qué hace la siguiente sentencia de C: PORTC |= (1<< PORTC0) | (1<< PORTC2) | (1<< PORTC3); ¿Cuál es la diferencia con la última del ejercicio anterior. ¿Qué representa PORTCx? ¿Dónde y cómo está definido?**

La sentencia ``PORTC |= (1<< PORTC0) | (1<< PORTC2) | (1<< PORTC3);`` modifica los terminales PC0, PC2 y PC3 en 1, sin modificar el resto.

(1 << PORTC0) = 00000001
(1 << PORTC2) = 00000100
(1 << PORTC3) = 00001000

PORTCx representa la posición x del puerto C.

**g) Escriba ahora una única sentencia para establecer el nivel lógico de PORTC0, PORTC2 y PORTC3 a nivel bajo sin afectar el estado lógico del resto de los bits del puerto.**

La sentencia que cumple con lo requerido es: ``PORTC &= ~((1<< PORTC0) | (1<< PORTC2) | (1<< PORTC3));``

Información: [Como setear pines en un cierto valor sin cambiar el resto](https://www.avrfreaks.net/s/topic/a5C3l000000UIr0EAG/t086268)