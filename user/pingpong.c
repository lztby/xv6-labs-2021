#include "kernel/types.h"
#include "kernel/stat.h"
#include "user/user.h"

int
main(int argc, char *argv[])
{
  int pid, p1[2], p2[2];
  char c;
  pipe(p1); // child read father write
  pipe(p2); // father read child write
  pid = fork();
  if(pid == 0) {
    close(p1[1]);
    close(p2[0]);
    if(read(p1[0], &c, 1) != 0) {
      fprintf(1, "%d: received ping\n", getpid());    
    }
    write(p2[1], "2", 1);
    close(p1[0]);
    close(p2[1]);
    exit(0);
  } else if (pid > 0) {
    close(p1[0]);
    close(p2[1]);
    write(p1[1], "1", 1);
    if(read(p2[0], &c, 1) != 0) {
      fprintf(1, "%d: received pong\n", getpid());    
    }
    close(p1[1]);
    close(p2[0]);
    wait(0); 
    exit(0);
  } else {
    fprintf(2, "usage: fork error\n");
  }
  exit(0);
}
