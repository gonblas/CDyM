#include <stdio.h>
#include <stdint.h>

void swap_values(int8_t *vec, uint32_t size) {
  size /= sizeof(int8_t);
  int8_t *left = vec, *right = vec + size - 1;
  int8_t temp;

  while (left < right) {
    temp = *left;
    *left = *right;
    *right = temp;
    left++;
    right--;
  }
}

int main() {
  int8_t vec[] = {1, 2, 3, 4, 5};
  swap_values(vec, sizeof(vec));
  for (int i = 0; i < 5; i++) {
    printf("%d, ", vec[i]);
  }
  getchar();
}
