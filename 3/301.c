#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <math.h>
#include <string.h>
#include <fcntl.h>
#include <unistd.h>

#define NUMBER_OF_MEASURE 5
#define SRC_FILE_NAME "big.txt"

void fgetc_copy() {
  char c;
  FILE *fps, *fpd;

  fps = fopen(SRC_FILE_NAME, "r");
  if(fps == NULL) {
    perror("fopen");
    exit(1);
  }

  fpd = fopen("fgetc_copy.txt", "w");
  if(fpd == NULL) {
    perror("fopen");
    fclose(fps);
    exit(1);
  }

  while ((c = fgetc(fps)) != EOF) {
    if(fputc(c, fpd) == EOF) {
      perror("fputc");
      fclose(fpd);
      fclose(fps);
      exit(1);
    }
  }

  fclose(fpd);
  fclose(fps);
}

void fgets_copy(char *buff, int buff_size) {
  FILE *fps, *fpd;

  fps = fopen(SRC_FILE_NAME, "r");
  if(fps == NULL) {
    perror("fopen");
    exit(1);
  }

  fpd = fopen("fgets_copy.txt", "w");
  if(fpd == NULL) {
    perror("fopen");
    fclose(fps);
    exit(1);
  }

  while (fgets(buff, buff_size, fps) != NULL) {
    if(fputs(buff, fpd) == EOF) {
      perror("fputs");
      fclose(fpd);
      fclose(fps);
      exit(1);
    }
  }

  fclose(fpd);
  fclose(fps);
}

void fread_copy(char *buff, int buff_size) {
  FILE *fps, *fpd;

  fps = fopen(SRC_FILE_NAME, "r");
  if(fps == NULL) {
    exit(1);
  }

  fpd = fopen("fread_copy.txt", "w");
  if(fpd == NULL) {
    perror("fopen");
    fclose(fps);
    exit(1);
  }

  int rcount;
  while (!feof(fps)) {
    rcount = fread(buff, 1, buff_size, fps);
    if(ferror(fps)) {
      perror("fread");
      fclose(fpd);
      fclose(fps);
      exit(1);
    }

    fwrite(buff, 1, rcount, fpd);
    if(ferror(fpd)) {
      perror("fwrite");
      fclose(fpd);
      fclose(fps);
      exit(1);
    }
  }

  fclose(fpd);
  fclose(fps);
}

void read_copy(char *buff, int buff_size) {
  char c;
  int fps, fpd;
  int count;

  fps = open(SRC_FILE_NAME, O_RDONLY);
  if(fps < 0) {
    perror("fopen");
    exit(1);
  }

  fpd = open("read_copy.txt", O_WRONLY | O_CREAT | O_TRUNC, 0644);
  if(fpd < 0) {
    perror("open");
    close(fps);
    exit(1);
  }

  while ((count = read(fps, buff, buff_size)) > 0) {
    if(write(fpd, buff, count) < 0) {
      perror("write");
      close(fpd);
      close(fps);
      exit(1);
    }
  }

  if(count < 0) {
    perror("read");
    close(fpd);
    close(fps);
    exit(1);
  }

  close(fpd);
  close(fps);
}

double average(double *results) {
  double sum = 0;
  int i;

  for(i = 0;i < NUMBER_OF_MEASURE;i++)
    sum += results[i];

  return sum / NUMBER_OF_MEASURE;
}

int main(int argc, char *argv[]) {
  struct timespec start_ts;
  struct timespec end_ts;
  double results[NUMBER_OF_MEASURE];
  char *buff;
  int buff_size;
  int i, j;

  if(argc != 3)
    exit(1);

  if((buff_size = atoi(argv[2])) == 0) {
    perror("second arument should be number");
    exit(1);
  };

  if((buff = (char *)malloc(buff_size * sizeof(char))) == NULL)
      exit(1);

  printf("%s, buff size: %d\n", argv[1], buff_size);
  if(strcmp(argv[1], "fgetc") == 0) {
    for(i = 0;i < NUMBER_OF_MEASURE;i++) {
      clock_gettime(CLOCK_MONOTONIC, &start_ts);
      fgetc_copy();
      clock_gettime(CLOCK_MONOTONIC, &end_ts);

      results[i] = (end_ts.tv_sec * pow(10, 9) + end_ts.tv_nsec) - (start_ts.tv_sec * pow(10, 9) + start_ts.tv_nsec);
      printf("%d: %lf[s]\n", i, results[i] * pow(10, -9));
    }
  } else if(strcmp(argv[1], "fgets") == 0) {
    for(i = 0;i < NUMBER_OF_MEASURE;i++) {
      clock_gettime(CLOCK_MONOTONIC, &start_ts);
      fgets_copy(buff, buff_size);
      clock_gettime(CLOCK_MONOTONIC, &end_ts);

      results[i] = (end_ts.tv_sec * pow(10, 9) + end_ts.tv_nsec) - (start_ts.tv_sec * pow(10, 9) + start_ts.tv_nsec);
      printf("%d: %lf[s]\n", i, results[i] * pow(10, -9));
    }
  } else if(strcmp(argv[1], "fread") == 0) {
    for(i = 0;i < NUMBER_OF_MEASURE;i++) {
      clock_gettime(CLOCK_MONOTONIC, &start_ts);
      fread_copy(buff, buff_size);
      clock_gettime(CLOCK_MONOTONIC, &end_ts);

      results[i] = (end_ts.tv_sec * pow(10, 9) + end_ts.tv_nsec) - (start_ts.tv_sec * pow(10, 9) + start_ts.tv_nsec);
      printf("%d: %lf[s]\n", i, results[i] * pow(10, -9));
    }
  } else if(strcmp(argv[1], "read") == 0) {
    for(i = 0;i < NUMBER_OF_MEASURE;i++) {
      clock_gettime(CLOCK_MONOTONIC, &start_ts);
      read_copy(buff, buff_size);
      clock_gettime(CLOCK_MONOTONIC, &end_ts);

      results[i] = (end_ts.tv_sec * pow(10, 9) + end_ts.tv_nsec) - (start_ts.tv_sec * pow(10, 9) + start_ts.tv_nsec);
      printf("%d: %lf[s]\n", i, results[i] * pow(10, -9));
    }
  }
  printf("average: %lf[s]\n", average(results) * pow(10, -9));
  printf("\n");

  free(buff);

  return 0;
}
