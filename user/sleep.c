#include "kernel/types.h"
#include "kernel/stat.h"
#include "user/user.h"

int
main(int argc, char *argv[])
{
  int t;
  if(argc <= 1){
    fprintf(2, "usage: sleep time\n");
    exit(1);
  }
  t = atoi(argv[1]);
  if (t < 0) {
    fprintf(2, "sleep: time %s is error \n", argv[1]);
  }
  sleep(t);
  exit(0);
}
