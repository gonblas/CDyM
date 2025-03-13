#include <stdio.h>

int sum(int8_t n){
  return n*(n+1)/2;
}

int main(int argc, char* argv[]){
  int8_t N = 6;
  printf("Suma de numeros de 1 hasta %d: %d", N, sum(N));
  
  return 0;
}