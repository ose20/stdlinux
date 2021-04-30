/*
  ex1.c

  タブ文字を'\t'に、改行を「$ + 改行」に置換して出力する
  cat コマンドの実装
*/

#include <stdio.h>
#include <stdlib.h>

static void do_cat(FILE *f);

int main(int argc, char *argv[]) {
  if (argc == 1) {
    do_cat(stdin);
  }
  else {
    for (int i = 1; i < argc; i ++) {
      FILE *f = fopen(argv[i], "r");
      if (!f) {
        perror(argv[i]);
        exit(1);
      }
      do_cat(f);
      fclose(f);
    }
  }
  exit(0);
}

static void do_cat(FILE *f) {
  int c;
  while ((c = fgetc(f)) != EOF) {
    switch (c) {
      case '\t':
        if (fputs("\\t", stdout) == EOF) exit(1);
        break;
      case '\n':
        if (fputs("$\n", stdout) == EOF) exit(1);
        break;
      default:
        if (fputc(c, stdout) == EOF) exit(1);
        break;
    }
  }
}
