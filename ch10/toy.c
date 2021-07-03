#include <stdio.h>
#include <string.h>


int main(int argc, char * argv[]) {
  int a = 5;
  int *p = &a;

  *p = 4;
  printf("the value which 'p' points is %d.\n", *p);
}
