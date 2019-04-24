#include <stdio.h>
#include <stdlib.h>
int DATA_SIZE = 10000;
int SEED = 10;

int comp_count = 0;

void swap(int *i, int *j) {
  int tmp;
  tmp = *i;
  *i = *j;
  *j = tmp;
}

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

int main() {
  int i;
  int data[DATA_SIZE];

  srand(SEED);
  for(i = 0;i < DATA_SIZE;i++)
    data[i] = rand();

  quicksort(data, 0, DATA_SIZE - 1);

  for(i = DATA_SIZE - 1;i >= 0;i--)
    printf("%d\n", data[1]);

  // printf("%d", comp_count);
  return 0;
}
