/*
  行数をカウントするコマンドの実装
*/

#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>

static void do_word_count(int fd, const char *path);
static void die(const char *s);

int main(int argc, char *argv[]) {
  if (argc < 2) {
    fprintf(stderr, "%s: file name not given\n", argv[0]);
    exit(1);
  }
  for (int i = 1; i < argc; i++) {
    char *path = argv[i];
    int fd = open(path, O_RDONLY);
    if (fd < 0) die(path);
    do_word_count(fd, path);
    if (close(fd) < 0) die(path);
  }
  exit(0);
}

#define BUFFER_SIZE 2048

static void do_word_count(int fd, const char *path) {
  unsigned long count = 0;
  for (;;) {
    unsigned char buf[BUFFER_SIZE];
    int n = read(fd, buf, sizeof(buf));
    if (n < 0) die(path);
    if (n == 0) break;
    for (int i = 0; i < n; i++) if (buf[i] == '\n') { count += 1; }
  }
  printf("input file has %lu lines.", count);
}

static void die(const char *s) {
  perror(s);
  exit(1);
}
