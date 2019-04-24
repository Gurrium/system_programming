#include <stdio.h>

int main() {
  FILE *fp;
  fp = fopen("hoge.txt", "r");

  int a[10];
  int i;

  for(i = 0;i < 10;i++) {
    fscanf(fp, "%d", &a[i]);
    printf("%d\n", a[i]);
  }
  return 0;
}
