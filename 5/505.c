#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <string.h>
#include <signal.h>
#include <unistd.h>
#include <sys/time.h>

void tmieout_handler(int signum) {
  exit(-2);
}

void sigint_handler(int signum) {
  exit(-3);
}

int mygetchar(int timeout_interval) {
  struct sigaction sa_timeout, sa_sigint;
  struct itimerval itimer;

  memset(&sa_timeout, 0, sizeof(sa_timeout));
  memset(&sa_sigint, 0, sizeof(sa_sigint));

  sa_timeout.sa_handler = tmieout_handler;
  sa_timeout.sa_flags = SA_RESTART;
  if(sigaction(SIGALRM, &sa_timeout, NULL) < 0) {
    perror("timeout sigaction");
    exit(1);
  }

  sa_sigint.sa_handler = sigint_handler;
  sa_sigint.sa_flags = SA_RESTART;
  if(sigaction(SIGINT, &sa_sigint, NULL) < 0) {
    perror("sigint sigaction");
    exit(1);
  }

  itimer.it_value.tv_sec = itimer.it_interval.tv_sec = timeout_interval;
  itimer.it_value.tv_usec = itimer.it_interval.tv_usec = 0;
  if(setitimer(ITIMER_REAL, &itimer, NULL) < 0) {
    perror("setitimer");
    exit(1);
  }

  char result = getchar();
  if(ferror(stdin)) {
    perror("getchar");
    exit(1);
  }

  itimer.it_value.tv_sec = itimer.it_interval.tv_sec = 0;
  itimer.it_value.tv_usec = itimer.it_interval.tv_usec = 0;
  if(setitimer(ITIMER_REAL, &itimer, NULL) < 0) {
    perror("resetitimer");
    exit(1);
  }

  sa_timeout.sa_handler = SIG_DFL;
  sa_timeout.sa_flags = SA_RESTART;
  if(sigaction(SIGALRM, &sa_timeout, NULL) < 0) {
    perror("sigaction");
    exit(1);
  }

  exit(result);
}

int main(int argc, char *argv[]) {
  if(argc != 2) {
    fprintf(stderr, "ERROR: lack of argument\nUSAGE: %s timeout_interval[s]", argv[0]);
    exit(1);
  }

  int timeout_interval;
  if((timeout_interval = atoi(argv[1])) == 0) {
    fprintf(stderr, "ERROR: timeout_interval should be greater than 0 and be integer");
    exit(1);
  }

  time_t current_time = time(NULL);
  if(current_time != -1) {
    printf("before calling mygetchar: %s", ctime(&current_time));
  }

  int is_parent;
  if((is_parent = fork()) < 0) {
    perror("fork");
    exit(1);
  }

  if(!is_parent) {
    mygetchar(timeout_interval);
  }

  signal(SIGINT, SIG_IGN);
  int status;
  if(wait(&status) < 0) {
    perror("wait");
    exit(1);
  }
  signal(SIGINT, SIG_DFL);

  printf("value returned from mygetchar is ");
  status = status >> 8;
  if('A' <= status && status <= 'z') {
    printf("%c", status);
  } else {
    printf("%d", status | 0xffffff00);
  }
  printf("\n");

  current_time = time(NULL);
  if(current_time != -1) {
    printf("after calling mygetchar: %s", ctime(&current_time));
  }

  return 0;
}
