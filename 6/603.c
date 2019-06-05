#include <stdio.h>
#include <stdlib.h>

int main(int argc, char *argv[]) {
  if(argc != 3) {
    fprintf(stderr, "Usage: %s host filename\n", argv[0]);
    exit(1);
  }

  char *host, *filename;
  host = argv[1];
  filename = argv[2];

  fprintf(stdout, "GET %s HTTP/1.0\r\n", filename);
  fprintf(stdout, "Host: %s\r\n", host);
  fprintf(stdout, "\r\n");

  return 0;
}
