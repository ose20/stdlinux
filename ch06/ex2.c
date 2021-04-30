/*
  ex2.c

  stdio API を使って、読み込んだファイルの行数を出力するコマンドを作成せよ。
  ファイル末尾に'/n'がない場合にも対応せよ。
*/

#include <stdio.h>
#include <stdlib.h>

static void do_wc_l(FILE *f);

int main(int argc, char *argv[]) {
  if (argc == 1) {
    do_wc_l(stdin);
  }
  else {
    for (int i = 1; i < argc; i++) {
      FILE *f = fopen(argv[i], "r");
      if(!f) {
        perror(argv[i]);
        exit(1);
      }
      do_wc_l(f);
      fclose(f);
    }
  }
  exit(0);
}

static void do_wc_l(FILE *f) {
  int c;
  int prev = '\n';

  unsigned long cnt = 0;
  while ((c = fgetc(f)) != EOF) {
    if (c == '\n') { cnt++; }
    prev = c;
  }
  if (prev != '\n') { cnt++; }

  printf("input has %lu line(s)\n", cnt);
}
