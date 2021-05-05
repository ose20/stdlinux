/*
  ex2.c

  正規表現に適合した部分文字列を出力するコマンドsliceの実装
*/

#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <regex.h>

static void do_slice(const regex_t *pat, FILE *f);

int main(int argc, char *argv[]) {

  // パターンが渡されていないとき
  if (argc < 2) {
    fputs("no pattern\n", stderr);
    exit(1);
  }

  regex_t pat;

  // パターンのコンパイルとエラーハンドリング
  int err = regcomp(&pat, argv[1], REG_EXTENDED | REG_NEWLINE);
  if (err != 0) {
    char buf[1024];

    regerror(err, &pat, buf, sizeof(buf));
    puts(buf);
    exit(1);
  }

  // 消化した引数分だけオフセットをずらす
  argc -= 2;
  argv += 2;

  // 本処理
  if (argc == 0) {
    do_slice(&pat, stdin);
  }
  else {
    for (int i = 0; i < argc; i++) {
      FILE *f = fopen(argv[i], "r");

      if (!f) {
        perror(argv[i]);
        exit(1);
      }
      do_slice(&pat, f);
      fclose(f);
    }
  }
  regfree(&pat);
  exit(0);
}

static void do_slice(const regex_t *pat, FILE *f) {
  char buf[4096];

  while (fgets(buf, sizeof buf, f)) {
    regmatch_t pmatch[1];
    if (regexec(pat, buf, 1, pmatch, 0) == 0) {
      char *str = buf + pmatch[0].rm_so;
      regoff_t len = pmatch[0].rm_eo - pmatch[0].rm_so;
      fwrite(str, len, 1, stdout);
      fputc('\n', stdout);
    }
  }
}
