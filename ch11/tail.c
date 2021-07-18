/*
  tail.c : 標準入力の末尾数行だけを出力するtailコマンドを書く。
  　　　　 表示する行数はコマンドラインオプションで受け取る。
*/

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

static void tail(FILE *f, int nlines);
static unsigned char *readline(FILE *f);

int main(int argc, char* argv[]) {
  if (argc != 2) {
    fprintf(stderr, "Usage: %s n\n", argv[0]);
    exit(1);
  }

  tail(stdin, atoi(argv[1]));
  exit(0);
}


#define INC(ptrvar) {ptrvar++; if (ptrvar >= ringbuf + nlines) ptrvar = ringbuf; }

static void tail(FILE *f, int nlines) {

  unsigned char **ringbuf;
  unsigned char **p;
  unsigned char *line;

  if (nlines == 0) return;

  ringbuf = p = calloc(nlines, sizeof(char*));
  if (ringbuf == NULL) exit(1);

  while((line = readline(f))) {
    if (*p) { free(*p); }
    *p = line;
    INC(p);
  }

  // 指定された行数より少ない行数しか読み込んでいない時の処理
  if (*p == NULL) { p = ringbuf; }
  for (int n = nlines; n > 0 && *p; n--) {
    printf("%s", *p);
    free(*p);
    INC(p);
  }
  free(ringbuf);
}


static unsigned char *readline(FILE *f) {

  unsigned char *buf, *p;
  size_t buflen = BUFSIZ;

  buf = p = malloc(sizeof(char) * buflen);
  if (buf == NULL) exit(1);
  int c;
  for (;;) {
    c = getc(f);
    if (c == EOF) {
      if (buf == p) {
        free(buf);
        return NULL;
      }
      break;
    }
    *p++ = c;
    if (p >= buf + buflen - 1) {
      buflen *= 2;
      buf = realloc(buf, buflen);
    }
    if (c == '\n') break;
  }
  *p++ = '\0';
  return buf;
}

