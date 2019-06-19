#include <stdio.h>
#include <stdlib.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <netdb.h>
#include <string.h>
#include <unistd.h>

#define PORTNO_BUFSIZE 30
#define READ_BUFSIZE 2048

int fdopen_sock(int sock, FILE **inp, FILE **outp) {
  int sock2 ;
  if((sock2 = dup(sock)) < 0) {
    return -1;
  }
  if((*inp = fdopen(sock2, "r")) == NULL) {
    close(sock2);
    return -1;
  }
  if((*outp = fdopen(sock, "w")) == NULL) {
    fclose(*inp);
    *inp = 0 ;
    return -1;
  }
  setvbuf(*outp, (char *)NULL, _IONBF, 0);

  return 0;
}

int tcp_connect(char *server, int portno) {
  struct addrinfo hints, *ai;
  char portno_str[PORTNO_BUFSIZE];
  int s, err;

  snprintf(portno_str, sizeof(portno_str), "%d", portno);
  memset(&hints, 0, sizeof(hints));
  hints.ai_socktype = SOCK_STREAM;
  if((err = getaddrinfo(server, portno_str, &hints, &ai))) {
    fprintf(stderr, "unknown server %s (%s)\n", server, gai_strerror(err));
    goto error0;
  }
  if((s = socket(ai->ai_family, ai->ai_socktype, ai->ai_protocol)) < 0) {
    perror("socket");
    goto error1;
  }
  if(connect(s, ai->ai_addr, ai->ai_addrlen) < 0) {
    perror(server);
    goto error2;
  }
  freeaddrinfo(ai);
  return s;
error2:
  close(s);
error1:
  freeaddrinfo(ai);
error0:
  return -1;
}

int main(int argc, char *argv[]) {
  if(argc != 4) {
    fprintf(stderr, "Usage: %s host port file\n", argv[0]);
    exit(1);
  }

  FILE *from_server, *to_server;
  char *host, *file;
  int port, sock;
  host = argv[1];
  port = strtol(argv[2], NULL, 10);
  file = argv[3];

  sock = tcp_connect(host, port);
  if(sock < 0) {
    exit(1);
  }
  if(fdopen_sock(sock, &from_server, &to_server) < 0) {
    fprintf(stderr, "fdopen()\n");
    close(sock);
    exit(1);
  }

  fprintf(to_server, "GET %s HTTP/1.1\r\n", file);
  fprintf(to_server, "Host: %s\r\n", host);
  fprintf(to_server, "\r\n");

  char buf[READ_BUFSIZE];
  while(fgets(buf, sizeof(buf), from_server) != NULL) {
    fprintf(stdout, "%s", buf);
  }
  if(ferror(from_server)) {
    fclose(to_server);
    fclose(from_server);
    fprintf(stderr, "ERROR: while reading data from %s%s\n", host, file);
    exit(1);
  }

  fclose(to_server);
  fclose(from_server);

  return 0;
}
