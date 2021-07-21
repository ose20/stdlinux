#include <stdio.h>
#include <stdlib.h>
#include <string.h>

struct toy {
  int (*f)(int);
};

static int succ(int x) {
  return x + 1;
}

static int decr(int x) {
  return x - 1;
}

int main(void) {

  struct toy toy_lst[] = {
    {succ},
    {decr}
  };

  printf("%d\n", toy_lst[0].f(100));
  exit(0);
}

