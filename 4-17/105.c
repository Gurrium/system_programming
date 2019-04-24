#include <stdio.h>
#include <stdlib.h>
int DATA_SIZE = 10000;

int comp_count = 0;

void quicksort(int target[], int left, int right) {
  int i, j, tmp;
  int pivot = target[right];

  comp_count++;
  if(left >= right)
    return;

  i = left;
  j = right - 1;
  while(1) {
    while(target[i] < pivot) {
      i++;
      comp_count++;
    }
    while(target[j] > pivot && j > i) {
      j--;
      comp_count++;
    }

    comp_count++;
    if(i >= j)
      break;

    tmp = target[i];
    target[i] = target[j];
    target[j] = tmp;

    i++;
    j--;
  }

  tmp = target[i];
  target[i] = target[right];
  target[right] = tmp;

  quicksort(target, left, i - 1);
  quicksort(target, i + 1, right);
}

int main(int argc, char *argv[]) {
  int i;
  int data[DATA_SIZE];
  FILE *file;
  file = fopen(argv[1], "r");

  for(i = 0;i < DATA_SIZE;i++)
    fscanf(file, "%d", &data[i]);

  quicksort(data, 0, DATA_SIZE - 1);

  printf("%d", comp_count);
  return 0;
}
