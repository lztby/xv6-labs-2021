#include "kernel/types.h"
#include "kernel/param.h"
#include "user/user.h"

// char buf[512];
#define NULL (void *) 0

char* 
readLine() 
{
    char c;
    static char buf[128];
    int pos = 0;
    while(read(0, &c, 1) != 0) {
        if (c == '\n') {
            break;
        } else {
            buf[pos++] = c;
        }
    }
    if (pos == 0) return NULL;
    // 可以用malloc 也可以用静态变量
    // char *s = (char*) malloc((pos) * sizeof(char));
    // memmove(s, buf, pos);
    // return s;
    buf[pos] = '\0';
    return buf;
}

int
main(int argc, char *argv[])
{
  if(argc < 2){
    fprintf(2, "usage: xargs cmd\n");
    exit(1);
  }
  int i;
  char *exec_argv[MAXARG];
  for (i = 1; i < argc; i++) {
    exec_argv[i - 1] = argv[i];
  }

  char *s;
  while((s = readLine()) != NULL) {
    // printf("buf [%s]\n", s);
    int pid = fork();
    if (pid == 0) {
      exec_argv[i - 1] = s;
      exec_argv[i] = 0;
      exec(exec_argv[0], exec_argv);
    } else if (pid > 0) {
      wait(0);
    }
  } 
  
/*
  int p[2];
  pipe(p); // father read child write
  if(fork() == 0) {
      close(p[1]);
      close(0);
      dup(p[0]); // 返回值一定是0 这样 p[0] 和 0 绑定
      exec(exec_argv[0], exec_argv);
  } else {
      // 写入p[1] p[0] 会收到消息
      close(p[0]);
      char buf[512];
      int n;
      while((n = read(0, buf, sizeof(buf))) > 0) {
        if (write(p[1], buf, n) != n) {
          break;
        }
      }
      close(p[1]); // 一定要关闭 否则read会一直阻塞
      wait(0);
  }
*/
  exit(0);
}


