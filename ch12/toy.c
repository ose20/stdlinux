#include <stdio.h>
#include <stdlib.h>
#include <string.h>

int main(void) {
  char *p;
  int a = 5;
  int *q =  &a;

  printf("%c\n", *p);
  printf("%p\n", q);
}

