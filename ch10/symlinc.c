#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

int main(int argc, char *argv[]) {

  if (argc != 3) {
    fprintf(stderr, "Usage: %s src dest\n", argv[0]);
    exit(0);
  }

  if (symlink(argv[1], argv[2]) < 0) {
    perror(argv[1]);
    exit(1);
  }

  exit(0);
}
