#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/uio.h>
#include <sys/wait.h>
#include <string.h>
#include <math.h>

int pipe_cp[2];
int pipe_pc[2];

void do_child(double threshold) {
  char *string = "0123456789";
  char control_char = ' ';
  char terminate_char = '\0';
  char c;
  int read_count, write_count, status;

  close(pipe_cp[0]);
  close(pipe_pc[1]);

  int i = 0;
  while(threshold--) {
    putchar(string[i++ % strlen(string)]);
    fflush(stdout);
    if(0 > threshold && threshold > -1) {
      write_count = write(pipe_cp[1], &terminate_char, 1);
      break;
    }
    if(!(write_count = write(pipe_cp[1], &control_char, 1) > 0))
      break;
    if(!(read_count = read(pipe_pc[0], &c, 1) > 0))
      break;
  }

  if(read_count < 0) {
    perror("read");
    exit(1);
  }

  if(write_count < 0) {
    perror("write");
    exit(1);
  }

  exit(0);
}

void do_parent() {
  char *string = "abcdefghijklmnopqrstuvwxyz";
  char control_char = ' ';
  char c;
  int read_count, write_count, status;

  close(pipe_cp[1]);
  close(pipe_pc[0]);

  int i = 0;
  while(1) {
    if(!(read_count = read(pipe_cp[0], &c, 1) > 0))
      break;
    if(c == '\0')
      break;
    putchar(string[i++ % strlen(string)]);
    fflush(stdout);
    if(!(write_count = write(pipe_pc[1], &control_char, 1) > 0))
      break;
  }

  if(read_count < 0) {
    perror("read");
    exit(1);
  }

  if(write_count < 0) {
    perror("write");
    exit(1);
  }

  if(wait(&status) < 0) {
    perror("wait");
    exit(1);
  }
}

int main(int argc, char *argv[]) {
  if(argc != 2) {
    perror("wrong argument");
    exit(1);
  }
  double threshold;
  if((threshold = (double)atoi(argv[1])) == 0) {
    fprintf(stderr, "USAGE: %s number_to_repeat", argv[0]);
    exit(1);
  }

  int child;

  if(pipe(pipe_cp) < 0) {
    perror("pipe_cp");
    exit(1);
  }

  if(pipe(pipe_pc) < 0) {
    perror("pipe_pc");
    exit(1);
  }

  if((child = fork()) < 0) {
    perror("fork");
    exit(1);
  }

  if(child) {
    do_parent();
  } else {
    do_child(threshold / 2);
  }

  return 0;
}
