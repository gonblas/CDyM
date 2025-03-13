#include <stdio.h>
#include <stdint.h>
#include <stdlib.h>
#define CERO_ASCII 48


char* number_to_char(uint8_t number) {
  uint8_t aux;
  int size;

  switch (number)
  {
  case 100 ... 255:
    size = 3;
    break;
  case 10 ... 99:
  size = 2;
    break;
  default:
    size = 1;
    break;
  }

  char* res = malloc(sizeof(char)*size);
  while(number > 0) {
    aux = number % 10;
    number = number / 10;
    res[--size] = aux + CERO_ASCII;
  }

  return res;
}



int main(int argc, char *argv[]) {
  uint8_t number = (uint8_t)atoi(argv[1]);

  char *res = number_to_char(number);
  for(int i=0; i < 3; i++){
    printf("%c ", res[i]);
  }
  getchar();
}