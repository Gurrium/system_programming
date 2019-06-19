#include <stdio.h>

int main() {
  FILE *f = fopen("hogeeee", "r");
  printf("%d", f == NULL);
  perror("fopen");

  return 0;
}
