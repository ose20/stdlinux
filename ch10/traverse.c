/*
  traverse.c :　引数として受け取ったディレクトリから再帰的に探索して、
  　　　　　　　見つかったファイルを出力する。シンボリックリンクの先はたどらない。
*/

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <errno.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <dirent.h>
#include <unistd.h>

struct strbuf {
  char *ptr;
  size_t len;
};


static struct strbuf *strbuf_new(void);
static void strbuf_realloc(struct strbuf *buf, size_t size);
static void print_error(char *s);
static void traverse(struct strbuf *buf);
static void traverse_aux(struct strbuf *buf, int is_first);

static const int INITLEN = 1024;
static char* program_name;

int main(int argc, char *argv[]) {

  if (argc != 2) {
    fprintf(stderr, "Usage: %s <dirname>\n", argv[0]);
    exit(1);
  }

  struct strbuf *pathbuf;

  program_name = argv[0];
  pathbuf = strbuf_new();
  strbuf_realloc(pathbuf, strlen(argv[1]+1));
  strcpy(pathbuf->ptr, argv[1]);
  traverse(pathbuf);
  exit(0);
}

static void traverse(struct strbuf *buf) {

  traverse_aux(buf, 1);
}

static void traverse_aux(struct strbuf *pathbuf, int is_first) {

  DIR *d = opendir(pathbuf->ptr);

  // dがディレクトリじゃなかった場合
  if (!d) {
    switch (errno) {
      case ENOTDIR:
        print_error(pathbuf->ptr);
        exit(1);
      case EACCES:
        puts(pathbuf->ptr);
        print_error(pathbuf->ptr);
        return;
      default:
        print_error(pathbuf->ptr);
        exit(1);
    }
  }

  // dがディレクトリの場合
  puts(pathbuf->ptr);
  struct dirent *ent;
  struct stat st;
  while((ent = readdir(d))) {
    if (strcmp(ent->d_name, ".") == 0) continue;
    if (strcmp(ent->d_name, "..") == 0) continue;
    strbuf_realloc(pathbuf, pathbuf->len + 1 + strlen(ent->d_name) + 1);

    {
      char *tmp = strrchr(pathbuf->ptr, '/');
      if (!tmp) {
        strcat(pathbuf->ptr, "/");
      }
      else if (*(tmp++) != '\0') {
        strcat(pathbuf->ptr, "/");
      }
    }
    strcat(pathbuf->ptr, ent->d_name);
    if (lstat(pathbuf->ptr, &st) < 0) {
      switch (errno) {
        case ENOENT:
          break;
        case EACCES:
          print_error(pathbuf->ptr);
          break;
        default:
          print_error(pathbuf->ptr);
          exit(1);
      }
    }
    else {
      if (S_ISDIR(st.st_mode) && !S_ISLNK(st.st_mode)) {
        traverse_aux(pathbuf, 0);
      }
      else {
        puts(pathbuf->ptr);
      }
    }
    *strrchr(pathbuf->ptr, '/') = '\0';
  }
  closedir(d);
}


static struct strbuf *strbuf_new(void) {

  struct strbuf *buf;

  buf = (struct strbuf*)malloc(sizeof(struct strbuf));
  if (!buf){
    print_error("malloc(3)");
    exit(1);
  }
  buf->ptr = malloc(INITLEN);
  if(!buf->ptr) {
  	print_error("malloc(3)");
    exit(1);
  }
  buf->len = INITLEN;
  return buf;
}

// bufの名前を格納する領域をlenバイト以上にする
static void strbuf_realloc(struct strbuf *buf, size_t len) {

  if (buf->len >= len) return;

  char *tmp = realloc(buf->ptr, len);
  if (!tmp) {
    print_error("realloc(3)");
    exit(1);
  }
  buf->ptr = tmp;
  buf->len = len;
  return;
}

// エラーが発生した時の処理
static void print_error(char *s) {

  fprintf(stderr, "%s: %s: %s\n", program_name, s, strerror(errno));
  return;
}
