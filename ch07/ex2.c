/*
  ファイルの最後の10行を出力するコマンドの実装
*/


#include <stdio.h>
#include <stdlib.h>

static void do_tail(FILE *f);
static int count_lines(FILE *f);

int main(int argc, char *argv[]) {
  if (argc == 1) {
    do_tail(stdin);
  }
  else {
    for (int i = 1; i < argc; i++) {
      FILE *f = fopen(argv[i], "r");
      if (!f) {
        perror(argv[i]);
        exit(1);
      }
      do_tail(f);
      fclose(f);
    }
  }
  exit(0);
}


static void do_tail(FILE *f) {
  int lines = count_lines(f);

  if (fseek(f, 0, SEEK_SET)) { exit(1); }

  int valid_line;
  if (lines < 10) { valid_line = 1; }
  else { valid_line = lines - 9; }

  int c;
  int tmp_line = 1;
  while ((c = getc(f)) != EOF) {
    if (tmp_line >= valid_line) {
      if (putc(c, stdout) == EOF) exit(1);
    }
    if (c == '\n') { tmp_line++; }
  }
}


static int count_lines(FILE *f) {
  int res = 0;
  int c;
  int prev = 'd'; // dummy
  while ((c = getc(f)) != EOF) {
    if (c == '\n') { res++; }
    prev = c;
  }
  if (prev != '\n') { res++; }

  return res;
}
