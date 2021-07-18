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
  int argc;
  char **argv;
  int capa;
  int status;
  int pid;
  struct cmd *next;
};

#define REDIRECT_P(cmd) ((cmd)->argc == -1)
#define PID_BUILTIN -2
#define BUILTIN_P(cmd) ((cmd)->pid == PID_BUILTIN)

struct builtin {
  char *name;
  // f : *((int, char*) -> int) という型
  int (*f)(int arg, char *argv[]);
};

static void prompt(void);
static int invoke_commands(struct cmd *cmd);
static void exec_pipeline(struct cmd *cmdhead);
static void redirect_stdout(char *path);
static int wait_pipeline(struct cmd *cmdhead);
static struct cmd* pipeline_tail(struct cmd *cmdhead);
static struct cmd* parse_command_line(char *cmdline);
static void free_cmd(struct cmd *p);
static struct builtin* lookup_builtin(char *name);
static int builtin_cd(int argc, char *argv[]);
static int builtin_pwd(int argc, char *argv[]);
static int builtin_exit(int argc, char *argv[]);
static void* xmalloc(size_t sz);
static void* xrealloc(void *ptr, size_t sz);

static char *program_name;

int main(int argc, char *argv[]) {
  program_name = argv[0];
  for (;;) {
    prompt();
  }
  exit(0);
}

#define LINEBUF_MAX 2048

static void prompt(void) {
  static char buf[LINEBUF_MAX];
  struct cmd *cmd;

  fprintf(stdout, "$ ");
  fflush(stdout);
  if (fgets(buf, LINEBUF_MAX, stdin) == NULL) {
    exit(0);
  }
  cmd = parse_command_line(buf);
  if (cmd == NULL) {
    fprintf(stderr, "%s: syntax error\n", program_name);
    return;
  }
  if (cmd->argc > 0) {
    invoke_commands(cmd);
  }
  free_cmd(cmd);
}

static int invoke_commands(struct cmd *cmdhead) {
  int original_stdin = dup(0);
  int st;
}





