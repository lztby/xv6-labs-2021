#include "kernel/types.h"
#include "kernel/stat.h"
#include "user/user.h"

int 
primeChild(int *p1) {
  int n, pid, p[2], nums[35];
  int pos = 0;
  while(read(p1[0], &n, 4) > 0) {
    nums[pos++] = n;
  } 
  if (pos == 0) {
    return 0;
  }
  pipe(p); // 0: read, 1: write
  pid = fork();
  if(pid == 0) {
    close(p[1]);
    primeChild(p);
    close(p[0]);
    exit(0);
  } else if (pid > 0) {
    close(p[0]);
    int prime = nums[0];
    printf("prime %d\n", prime);
    for (int i = 0; i < pos; i++) {
      if (nums[i] % prime != 0) {
        write(p[1], &nums[i], 4);
      }
    }
    close(p[1]);
    wait(0); 
    exit(0);
  } else {
    fprintf(2, "usage: fork error\n");
  }
  return 0;
}

int
main(int argc, char *argv[])
{
  int pid, p[2];
  pipe(p); // 0: read, 1: write
  pid = fork();
  if(pid == 0) {
    close(p[1]);
    primeChild(p);
    close(p[0]);
    exit(0);
  } else if (pid > 0) {
    close(p[0]);
    for (int i = 2; i <= 35; i++) {
        write(p[1], &i, 4);
    }
    close(p[1]);
    wait(0); 
    exit(0);
  } else {
    fprintf(2, "usage: fork error\n");
  }
  exit(0);
}
