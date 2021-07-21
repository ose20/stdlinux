#include <stdio.h>
#include <unistd.h>

int main(void) {
  int fd[2] = {-1, -1};

  pipe(fd);
  printf("%d %d\n", fd[0], fd[1]);
}
