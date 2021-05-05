/*
  ex1.c

  -iオプションと-vオプションのあるgrepコマンドの実装
*/

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <regex.h>

static void grep_file(const regex_t *re, const int *opt_invert, const char *path);
static void grep_stream(const regex_t *re, const int *opt_invert, FILE *f);

int main(int argc, char *argv[]) {

  int opt;
  int opt_invert = 0;
  int opt_ignorecase = 0;

  while ((opt = getopt(argc, argv, "iv")) != -1) {
    switch (opt) {
      case 'i':
        opt_ignorecase = 1;
        break;
      case 'v':
        opt_invert = 1;
        break;
      case '?':
        fprintf(stderr, "Usage: %s [-iv] [<file> ... ]\n", argv[0]);
        exit(1);
    }
  }

  argc -= optind;
  argv += optind;

  if (argc < 1) {
    fputs("no pattern\n", stderr);
    exit(1);
  }

  char *pattern = argv[0];
  argc--;
  argv++;

  int re_mode = REG_EXTENDED | REG_NOSUB | REG_NEWLINE;
  if (opt_ignorecase) { re_mode |= REG_ICASE; }
  regex_t re;
  int err = regcomp(&re, pattern, re_mode);
  if (err != 0) {
    char buf[1024];

    regerror(err, &re, buf, sizeof buf);
    puts(buf);
    exit(1);
  }

  if (argc == 0) {
    grep_stream(&re, &opt_invert, stdin);
  }
  else {
    for (int i = 0; i < argc; i++) {
      grep_file(&re, &opt_invert, argv[i]);
    }
  }
  regfree(&re);
  exit(0);
}


static void grep_file(regex_t const *re, int const *opt_invert, const char *path) {
  FILE *f = fopen(path, "r");
  if (!f) {
    perror(path);
    exit(1);
  }
  grep_stream(re, opt_invert, f);
  fclose(f);
}


static void grep_stream(regex_t const *re, int const *opt_invert, FILE *f) {
  char buf[4096];

  while (fgets(buf, sizeof buf, f)) {
    int matched = regexec(re, buf, 0, NULL, 0);
    if (!opt_invert) { matched = !matched; }
    if (matched) { fputs(buf, stdout); }
  }
}
