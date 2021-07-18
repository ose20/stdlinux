#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/stat.h>
#include <sys/errno.h>

static void make_path(const char *path);
static void die(const char *s);

int main(int argc, char* argv[]) {

  if (argc < 2) {
    fprintf(stderr, "%s: no arguments\n", argv[0]);
    exit(1);
  }

  for(int i=1; i < argc; i++) {
    make_path(argv[i]);
  }
  exit(0);
}


static void make_path(const char *path) {

  if (mkdir(path, 0777) == 0) {
    // 問題なく終了
    return;
  }

  if (errno == EEXIST) {
    struct stat st;
    if (stat(path, &st) < 0) die("stat");
    if (!S_ISDIR(st.st_mode)) {
      fprintf(stderr, "file already exists but is not a directory: %s\n", path);
      exit(1);
    }
    return;
  }
  else if (errno == ENOENT) {
    // 親ディレクトリがないので作る
    char *parent_path = strdup(path);
    if (!parent_path) die("strdup");

    // 後ろから連続する'/'を削除
    char *last = parent_path + strlen(parent_path) - 1;
    while (*last == '/' && last != parent_path) {
      *last-- = '\0';
    }

    if (strcmp(parent_path, "/") == 0) {
      fprintf(stderr, "error: we happened to make '/' directory.\n");
      exit(1);
    }

    char *sep = strrchr(parent_path, '/');
    if (!sep) {
      fprintf(stderr, "error: cant find '/' in pathname.\n");
      exit(1);
    }
    else if (sep == parent_path) {
      fprintf(stderr, "error: root directory is not a directory???\n");
      exit(1);
    }
    *sep = '\0';

    make_path(parent_path);
    if(mkdir(path, 0777) < 0) die(path);
    return;
  }
  else {
    die(path);
  }
}

static void die(const char *s) {

  perror(s);
  exit(1);
}
