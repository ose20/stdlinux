#include <stdio.h>
#include <stdlib.h>

int main (int argc, char *argv[]) {

  for(int i = 0; i < 100; i++) {
    printf("\007");
  }
  exit(0);
}
