#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <dirent.h>

static void do_ls(char *path);

int main(int argc, char *argv[]) {
  if (argc < 2) {
    fprintf(stderr, "%s: no arguments\n", argv[0]);
    exit(1);
  }

  for (int i = 1; i < argc; i++) {
    do_ls(argv[i]);
  }
  exit(0);
}

static void do_ls(char *path) {

  DIR *d = opendir(path);
  if (!d) {
    perror(path);
    exit(1);
  }

  struct dirent *ent;
  struct dirent *firstent;
  int cnt = 0;
  while ((ent = readdir(d)) != NULL) {
    if(cnt++ == 0) {firstent = ent;}
    printf("%s\n", ent->d_name);
  }
  printf("first entry name is %s\n", firstent->d_name);
  closedir(d);
}
