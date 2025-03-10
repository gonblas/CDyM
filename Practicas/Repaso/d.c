#include <stdio.h>
#include <stdint.h>
#include <stdlib.h>
#include <unistd.h>


void print_bin(int8_t N) {
  int aux = 0b1;
  for(int i=0; i<8; i++) {
    printf("%d ", aux & N);
    N = N >> 1;
    sleep(1);
  }
}


int main(int argc, char *argv[]) {
  print_bin(0b01010101);
  getchar();
}