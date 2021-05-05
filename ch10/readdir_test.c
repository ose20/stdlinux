/*
  p197 に 「戻り値のポインタが指す struct dirent は、次に readdir() を
  読んだ時点で上書きされるので注意してください。つまり、readdir() で得た
  struct dirent をどこかに保存して置いてあとで使うようなことはできません。」
  とあるが、信じ難いので試してみる。
*/

#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <dirent.h>

static void do_ls(const char *path);

int main(int argc, char *argv[]) {

  if (argc < 2) {
    fprintf(stderr, "%s: no arguments\n", argv[0]);
    exit(1);
  }

  for (int i = 1; i < argc; i ++) {
    do_ls(argv[i]);
    exit(0);
  }
}


static void do_ls(const char *path) {

  DIR *d = opendir(path);
  if (!d) {
    perror(path);
    exit(1);
  }

  struct dirent *ent[10];
  int cnt = 0;
  struct dirent *tmp;
  while ((tmp = readdir(d)) != NULL && cnt < 10) {
    ent[cnt] = tmp;
    cnt++;
  }

  for(int i = 0; i < cnt; i++) {
    printf("%s\n", ent[i]->d_name);
  }
  closedir(d);
}
