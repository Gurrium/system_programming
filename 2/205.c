#include <stdio.h>
#include <stdlib.h>
#include <string.h>

size_t my_strlcpy(char *dst, const char *src, size_t size) {
  size_t i = 0;

  for(i = 0;*src != '\0' && i < size - 1;i++)
    *dst++ = *src++;
  *dst = '\0';

  return strlen(src - i);
}

char *my_strdup(const char *s) {
  char *t = (char *)malloc((strlen(s) + 1) * sizeof(char));
  char *result = t;

  while(*s != '\0')
    *t++ = *s++;
  *t = *s; // '\0'

  return result;
}

int main() {
  char s1[] = "a";
  char s2[] = "hoge";
  char s3[] = "fugafugafuga";
  char *strings[] = {s1, s2, s3};
  int i;
  char buf[5];
  char *dupbuf;

  for(i = 0;i < 3;i++) {
    printf("%d: %s\n", i + 1, strings[i]);
    printf("strlcpy\nreturned value: %lu, string: %s\n", strlcpy(buf, strings[i], sizeof(buf)), buf);
    printf("my_strlcpy\nreturned value: %lu, string: %s\n\n", my_strlcpy(buf, strings[i], sizeof(buf)), buf);
    printf("strdup\nstring starts from returned pointer: %s\n", strdup(strings[i]));
    printf("my_strdup\nstring starts from returned pointer: %s\n\n", dupbuf = my_strdup(strings[i]));
  }

  free(dupbuf);

  return 0;
}
