/*
  '\t' と '\n' を可視化する昨日を cat コマンドの
  オプションとして使えるようにする
*/

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

static void do_cat(FILE *f, int const escape);

int main(int argc, char *argv[]) {

  int escape = 0;
  int opt;
  while ((opt = getopt(argc, argv, "e")) != -1) {
    switch (opt) {
      case 'e':
        escape = 1;
        break;
      case '?':
        fprintf(stderr, "Usage: %s [-e] [FILE ...]\n", argv[0]);
        exit(1);
    }
  }

  if (optind == argc) {
    do_cat(stdin, escape);
  }
  else {
    for (int i = optind; i < argc; i++) {
      FILE *f = fopen(argv[i], "r");
      if (!f) {
        perror(argv[i]);
        exit(1);
      }
      do_cat(f, escape);
      fclose(f);
    }
  }
  exit(0);
}

static void do_cat(FILE *f, int const escape) {
  int c;
  while ((c = getc(f)) != EOF) {
    if(escape) {
      if (c == '\t') {
        if (fputs("\\t", stdout) == EOF) exit(1);
      }
      else if (c == '\n') {
        if (fputs("$\n", stdout) == EOF) exit(1);
      }
      else {
        if (putc(c, stdout) == EOF) exit(1);
      }
    }
    else {
      if (putc(c, stdout) == EOF) exit(1);
    }
  }
}
