#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#define MAX_FILE_PATH_LENGTH 2048
#define MAX_LINE_LENGTH 1048576

extern void http_send_reply(FILE *out, char *filename);
extern void http_send_reply_success(FILE *out, FILE *requested_file);
extern void http_send_reply_bad_request(FILE *out);
extern void http_send_reply_not_found(FILE *out);

int main(int argc, char *argv[]) {
  char *filename;

  if(argc != 2) {
    fprintf(stderr,"Usage: %s filename\n",argv[0]);
    exit(1);
  }
  filename = argv[1];
  http_send_reply(stdout, filename);
}

void http_send_reply(FILE *out, char *filename) {
  char *ext;
  char file_path[MAX_FILE_PATH_LENGTH];
  FILE *requested_file;

  ext = strrchr(filename, '.');
  if(ext == NULL) {
    http_send_reply_bad_request(out);
    return;
  } else if(strcmp(ext,".html") == 0) {
    if(filename[0] == '/') filename++;
    snprintf(file_path, MAX_FILE_PATH_LENGTH, "%s/%s", getenv("SYSPRO_HOME"), filename);
    printf("filename: [%s], extention: [%s], path: [%s].\n", filename, ext, file_path);

    if((requested_file = fopen(file_path, "r")) == NULL) {
      perror("fopen");
      http_send_reply_not_found(out);
      return;
    }
    http_send_reply_success(out, requested_file);
    fclose(requested_file);
    return;
  } else {
    http_send_reply_bad_request(out);
    return;
  }
}

void http_send_reply_success(FILE *out, FILE *requested_file) {
  fprintf(out, "HTTP/1.0 200 OK\r\nContent-Type: text/html\r\n\r\n");

  char buff[MAX_LINE_LENGTH];
  int rcount;
  while (!feof(requested_file)) {
    rcount = fread(buff, 1, MAX_LINE_LENGTH, requested_file);
    if(ferror(requested_file)) {
      perror("fread");
      exit(1);
    }

    fwrite(buff, 1, rcount, out);
    if(ferror(out)) {
      perror("fwrite");
      exit(1);
    }
  }
}

void http_send_reply_internal_server_error(FILE *out) {
  fprintf(out,"HTTP/1.0 500 Internal Server Error\r\nContent-Type: text/html\r\n\r\n");
  fprintf(out,"<html><head></head><body>500 Internal Server Error</body></html>\n");
}

void http_send_reply_bad_request(FILE *out) {
  fprintf(out,"HTTP/1.0 400 Bad Request\r\nContent-Type: text/html\r\n\r\n");
  fprintf(out,"<html><head></head><body>400 Bad Request</body></html>\n");
}

void http_send_reply_not_found(FILE *out) {
  fprintf(out,"HTTP/1.0 404 Not Found\r\nContent-Type: text/html\r\n\r\n");
  fprintf(out,"<html><head></head><body>404 Not Found</body></html>\n");
}
