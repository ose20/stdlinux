
#include <stdio.h>
#include <stdlib.h>

static void inc() {
  p++;
}

int main(){

  char p[10] = "string";
  inc(p); inc(p);
  printf("%c\n", *p);

  exit(0);
}
