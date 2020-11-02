#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <sys/types.h>
#include <sys/uio.h>
#include <unistd.h>
#include <ctype.h>

extern void print_header(void);
extern void print_body(void);
extern void print_html_body(void);
extern void print_cal(int year, int month);
extern char *get_query_string();
extern char *read_query_string();
extern void safe_print_string( char *str );
extern char *html_escape( char *str );
extern char *getparam( int qc, char *qv[], char *name );

extern int string_split( char *str, char del, int *countp, char ***vecp  );
extern void free_string_vector( int qc, char **vec );
extern int countchr( char *s, char c );

int main() {
  print_header();
  print_body();

  return 0;
}

void print_header() {
  printf("Content-Type: text/html\n");
  printf("\n");
  fflush(stdout);
}

void print_body() {
  printf("<!DOCTYPE html><html lang=\"en\"><head><meta charset=\"UTF-8\"><title>cgi test</title></head><body>\n");
  fflush(stdout);
  print_html_body();
  printf("</body> </html>");
  fflush(stdout);
}

void print_html_body() {
  char *query_string;
  char **qv;
  int qc;

  if((query_string = get_query_string()) == 0) {
    printf("Error while parsing queries");
    return;
  }

  if(string_split(query_string, '&', &qc, &qv) < 0 ) {
    if(query_string)
      free(query_string);
    printf("Error while parsing queries");
    return;
  }

  if(qc != 2) {
    printf("Wrong number of params");
    return;
  }

  char *year_s, *month_s;
  year_s = getparam(qc, qv, "year");
  month_s = getparam(qc, qv, "month");
  if(year_s == NULL || month_s == NULL) {
    printf("Wrong number of params");
    return;
  }

  int year, month;
  year = atoi(year_s);
  month = atoi(month_s);
  if(!(1 <= year && year <= 9999)) {
    printf("year is invalid");
    return;
  }
  if(!(1 <= month && month <= 12)) {
    printf("month is invalid");
    return;
  }

  printf("<pre>");
  fflush(stdout);
  print_cal(year, month);
  printf("</pre>");
  fflush(stdout);

  free_string_vector(qc, qv);
  if(query_string)
    free(query_string);
}

void print_cal(int year, int month) {
  int child, status;

  if((child = fork()) < 0) {
    perror("fork");
    exit(1);
  }

  if(child) {
    if(wait(&status) < 0) {
      perror("wait");
      exit(1);
    }
  } else {
    char *argv[] = {"/usr/bin/cal", "-hm", "", "", NULL};
    char *const envp[] = {NULL};
    argv[2] = (char *)malloc(3);
    argv[3] = (char *)malloc(5);

    snprintf(argv[2], 3, "%d", month);
    snprintf(argv[3], 5, "%d", year);

    execve("/usr/bin/cal", argv, envp);
    free(argv[2]);
    free(argv[3]);
    exit(0);
  }
}

char *get_query_string() {
  char *request_method, *query_string;
  request_method = getenv("REQUEST_METHOD");
  if( request_method == 0 )
    return( 0 );
  else if( strcmp(request_method,"GET") == 0 ) {
    query_string = getenv("QUERY_STRING");
    if( query_string == 0 )
      return( 0 );
    else
      return( strdup(query_string) );
  }
  else if( strcmp(request_method,"POST") == 0 ) {
    return( read_query_string() );
  }
  else {
    printf("Unknown method: ");
    safe_print_string( request_method );
    printf("\n");
    return( 0 );
  }
}

char * read_query_string() {
  int   clen ;
  char *content_length ;
  char *buf ;

  content_length = getenv("CONTENT_LENGTH");
  if( content_length == 0 ) {
    return( 0 );
  }
  else {
    clen = strtol( content_length,0,10 );
    buf = malloc( clen + 1 );
    if( buf == 0 ) {
      printf("read_query_string(): no memory\n");
      exit( -1 );
    }
    if( read(0,buf,clen) != clen ) {
      printf("read error.\n");
      exit( -1 );
    }
    buf[clen] = 0 ;
    return( buf );
  }
}


void safe_print_string( char *str ) {
  char *safe_str ;

  if( str == 0 ) {
    printf("(null)");
    return;
  }
  safe_str = html_escape( str );
  if( safe_str == 0 ) {
    printf("(no memory)");
  }
  else {
    printf("%s",safe_str );
    free( safe_str );
  }
}

char * html_escape( char *str ) {
  int len ;
  char c, *tmp, *p, *res ;

  len = strlen( str );
  tmp = malloc( len * 6 + 1 );
  if( tmp == 0 )
    return( 0 );
  p = tmp ;
  while( (c = *str++) ) {
    switch( c )
    {
      case '&': memcpy(p,"&amp;", 5); p += 5 ; break;
      case '<': memcpy(p,"&lt;",  4); p += 4 ; break;
      case '>': memcpy(p,"&gt;",  4); p += 4 ; break;
      case '"': memcpy(p,"&quot;",6); p += 6 ; break;
      default:  *p = c ;              p++ ;    break;
    }
  }
  *p = 0 ;
  res = strdup( tmp );
  free( tmp );
  return( res );
}

char *getparam( int qc, char *qv[], char *name ) {
  int i;
  size_t len;

  len = strlen(name);
  for(i = 0;i < qc;i++) {
    if(strncmp(qv[i], name, len) == 0 && qv[i][len] == '=') {
      return(&qv[i][len+1]);
    }
  }
  return NULL;
}

int string_split( char *str, char del, int *countp, char ***vecp  ) {
  char **vec ;
  int  count_max, i, len ;
  char *s, *p ;

  if( str == 0 )
    return( -1 );
  count_max = countchr(str,del)+1 ;
  vec = malloc( sizeof(char *)*(count_max+1) );
  if( vec == 0 )
    return( -1 );

  for( i=0 ; i<count_max ; i++ ) {
    while( *str == del )
      str ++ ;
    if( *str == 0 )
      break;
    for( p = str ; *p!=del && *p!=0 ; p++ )
      continue;
    /* *p == del || *p=='\0' */
    len =  p - str ;
    s = malloc( len+1 );
    if( s == 0 ) {
      int j ;
      for( j=0 ; j<i; j++ ) {
        free( vec[j] );
        vec[j] = 0 ;
      }
      return( -1 );
    }
    memcpy( s, str, len );
    s[len] = 0 ;
    vec[i] = s ;
    str = p ;
  }
  vec[i] = 0 ;
  *countp = i ;
  *vecp = vec ;
  return( i );
}

void free_string_vector( int qc, char **vec ) {
  int i ;
  for( i=0 ; i<qc ; i++ ) {
    if( vec[i] == NULL )
      break;
    free( vec[i] );
  }
  free( vec );
}

int countchr( char *s, char c ) {
  int count ;
  for( count=0 ; *s ; s++ )
    if( *s == c )
      count ++ ;
  return( count );
}
