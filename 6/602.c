#include <stdio.h>

int main() {
  fprintf(stdout, "GET /index.html HTTP/1.0\r\n");
  fprintf(stdout, "Host: www.coins.tsukuba.ac.jp\r\n");
  fprintf(stdout, "\r\n");

  return 0;
}
