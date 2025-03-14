# Ejercicio No 1: Repaso de lenguaje C

**a) Investigue sobre los distintos tipos de variables en C y sus modificadores: static, volatile, register, const. ¿Cuáles son los tipos de datos por defecto en el compilador que utilizaremos?**

Los principales tipos de variables en C son:

|  Nombre  |                  Uso                  |
| :------: | :-----------------------------------: |
|  `int`   |            Números enteros            |
| `float`  |           Números flotantes           |
| `double` | Números flotantes con doble precisión |
|  `char`  |              Caracteres               |
|  `void`  |          No representa nada           |


Los modificadores son:

|   Nombre   |                                                                    Uso                                                                     |
| :--------: | :----------------------------------------------------------------------------------------------------------------------------------------: |
|  `static`  |  La variable mantiene el su valor entre llamadas a funciones. La variable se elimina cuando termina el programa. Util para recursividad.   |
| `volatile` |     Le avisa al compilador que el valor de la variable puede cambiar inesperadamente. Utilizado en sistemas embebidos y concurrentes.      |
| `register` | Le indica al compilador que puede poner la variable en un registro de la CPU. Utilizado en variables que se deben acceder recurrentemente. |
|  `const`   |                                            El valor de la variable no cambia. De solo lectura.                                             |





**b) Investigue sobre las sentencias del pre-procesador de C, entre ellas: #include, #define, #ifdef y typedef.**

|   Nombre   |                            Uso                            |
| :--------: | :-------------------------------------------------------: |
| `#include` |        Incluir archivos de cabecera en el código.         |
| `#define`  |                Define macros o constantes.                |
|  `#ifdef`  | Ejecución condicional en base a si una macro existe o no. |
| `typedef`  |   Crea alias para tipos de datos. Utilizado en structs.   |



**c) ¿Qué es una constante de carácter? ¿qué es una cadena de caracteres?**

La constantes de caŕacter en un valor numerico que representa un valor segun la tabla ASCII.


**d) ¿Cuál es la diferencia entre una variable local y una global? ¿Por qué utilizaría una u otra?**

La diferencia entre una variable local y una global es desde donde pueden ser accedidas. Las variables globales se pueden acceder desde cualquier parte, mientras que las variables globales se pueden acceder dentro de la función en la cual es declarada. En general utilizaría variables locales, puesto que es una buena práctica. Las variables globales cuando muchas funciones requieren de un mismo valor o para configuraciones globales.


**e) Describa todos los operadores lógicos de C. ¿cúal es la diferencia entre los operadores && y &, || y |? ¿Qué es una máscara de bits?**


| Operador |          Uso          |
| :------: | :-------------------: |
|   `&&`   |      AND lógico       |
|  `\|\|`  |       OR lógico       |
|   `!`    |      NOT lógico       |
|   `&`    |     AND bit a bit     |
|   `\|`   |     OR bit a bit      |
|   `^`    |     XOR bit a bit     |
|   `~`    | Complemento bit a bit |


Las operaciones `&&` y `\|\|` son operaciones lógicas, se utilizan generalmente en condiciones de estructuras de control, mientras que `&`y `\|` son operaciones de bits.  

**f) ¿Qué es un prototipo de función en C? ¿Cuáles son las alternativas para pasar argumentos a una función? ¿Cómo se retorna un valor desde una función?**

Un prototipo de función declara el tipo de retorno, el nombre y los parámetros de una función antes de su definición. Hace que sea más legible y permite verificar los tipos de datos de los parámetros y el tipo de dato que se retorna, y evitar errores.

|         Método         |                                 Descripción                                  |
| :--------------------: | :--------------------------------------------------------------------------: |
|    Por valor (type)    |       Se pasa una copia del valor, la función no modifica el original.       |
| Por referencia (type*) | Se pasa la dirección de la variable, la función puede modificar el original. |

con `return` se devuelve un tipo de dato.


**g) Repase el concepto de punteros y arreglos. Explique con ejemplos la relación entre ambos.**

El nombre de un arreglo es un puntero al primer elemento. Fin.


**h) Investigue sobre los tipos de variables struct y union (estructuras y uniones) en C. De un ejemplo de cada caso.**

El `struct` agrupa múltiples variables bajo un mismo nombre, permitiendo almacenar diferentes tipos de datos. Cada miembro ocupa su propio espacio en memoria.

```c
#include <stdio.h>

struct Persona {
    char nombre[20];
    int edad;
};

int main() {
    struct Persona p = {"Juan", 25};
    printf("Nombre: %s, Edad: %d\n", p.nombre, p.edad);
    return 0;
}
```

El `union` almacena diferentes tipos de datos en la misma ubicación de memoria. Solo un miembro puede contener un valor a la vez.


```c
#include <stdio.h>

union Dato {
    int i;
    float f;
};

int main() {
    union Dato d;
    d.i = 10;
    printf("Entero: %d\n", d.i);
    d.f = 3.14;
    printf("Flotante: %f\n", d.f); // Sobrescribe el valor de `i`
    return 0;
}
```


**i) ¿Qué son los campos de bit de una estructura?**


Los campos de bit permiten asignar un número específico de bits a los miembros de una estructura. Esto es útil para ahorrar espacio en memoria cuando se necesita almacenar información que no requiere de un byte completo o más.