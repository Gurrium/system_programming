#include <stdio.h>
#include <stdlib.h>
int DATA_SIZE = 10000;

int comp_count = 0;

void swap_array(int a[], int i, int j) {
  int tmp;

  tmp = a[i];
  a[i] = a[j];
  a[j] = tmp;
}

void sort(int data[], int count) {
  int i, j;
  int n = count - 1;

  for (i = 0; i < n; i++) {
    comp_count++;

    for (j = 0; j < n - i; j++) {
      comp_count++;

      if (data[j] > data[j + 1]) {
        comp_count++;

        swap_array(data, j, j + 1);
      }
    }
  }
}

int main(int argc, char *argv[]) {
  int i;
  int data[DATA_SIZE];
  FILE *file;
  file = fopen(argv[1], "r");

  for(i = 0;i < DATA_SIZE;i++)
    fscanf(file, "%d", &data[i]);

  sort(data, DATA_SIZE);

  printf("%d", comp_count);
  return 0;
}
