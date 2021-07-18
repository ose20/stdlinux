/*
  sh1.c: コマンドを実行するREPLを実装する
*/

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <sys/wait.h>
#include <fcntl.h>
#include <limits.h>
#include <ctype.h>
#include <string.h>

struct cmd {
  char **argv;
  long argc;
  long capa;
};

static void invoke_cmd(struct cmd *cmd);
static struct cmd* read_cmd(void);
static struct cmd* parse_cmd(char *cmdline);
static void free_cmd(struct cmd *p);
static void* xmalloc(size_t sz);
static void* xrealloc(void *ptr, size_t sz);

static char *program_name;

#define PROMPT "$ "

int main(int argc, char *argv[]) {
  program_name = argv[0];

  for (;;) {
    struct cmd *cmd;

    fprintf(stdout, PROMPT);
    fflush(stdout);
    cmd = read_cmd();
    if (cmd->argc > 0) {
      invoke_cmd(cmd);
    }
    free_cmd(cmd);
  }
  exit(0);
}

static void invoke_cmd(struct cmd *cmd){
  pid_t pid;

  pid = fork();
  if (pid < 0) {
    perror("fork");
    exit(1);
  }
  if (pid > 0) {
    // 親プロセス
    waitpid(pid, NULL, 0);
  }
  else {
    // 子プロセス
    execvp(cmd->argv[0], cmd->argv);
    // error
    fprintf(stderr, "%s: command not found: %s\n", program_name, cmd->argv[0]);
    exit(1);
  }
}

#define LINE_BUF_SIZE 2048

static struct cmd *read_cmd(void) {
  // 関数の中でのstatic変数なので、これは関数呼び出しが終わっても消滅しない
  static char buf[LINE_BUF_SIZE];

  if (fgets(buf, LINE_BUF_SIZE, stdin) == NULL) {
    // Ctrl-D(EOF) で終われるようにする。
    exit(0);
  }
  return parse_cmd(buf);
}

#define INIT_CAPA 16

// 文字列を解析してcmd型に変換
static struct cmd* parse_cmd(char *cmdline) {
  char *p = cmdline;
  struct cmd *cmd;

  cmd = xmalloc(sizeof(struct cmd));
  cmd->argc = 0;
  cmd->argv = xmalloc(sizeof(char*) * INIT_CAPA);
  cmd->capa = INIT_CAPA;

  while (*p) {
    while (*p && isspace((int)*p)) {
      *p++ = '\0';
    }
    if (*p) {
      if (cmd->capa <= cmd->argc + 1) {
        cmd->capa *= 2;
        cmd->argv = xrealloc(cmd->argv, cmd->capa);
      }
      cmd->argv[cmd->argc] = p;
      cmd->argc++;
    }
    while (*p && !isspace((int)*p)) {
      p++;
    }
  }
  cmd->argv[cmd->argc] = NULL;
  return cmd;
}


static void free_cmd(struct cmd *cmd) {
  free(cmd->argv);
  free(cmd);
}


static void* xmalloc(size_t size) {
  void *p;

  p = malloc(size);
  if (!p) {
    perror("malloc");
    exit(1);
  }
  return p;
}

static void* xrealloc(void *ptr, size_t size) {
  void *p;

  if (!ptr) return xmalloc(size);
  p = realloc(ptr, size);
  if (!p) {
    perror("realloc");
    exit(1);
  }
  return p;
}
