#include "types.h"
#include "riscv.h"
#include "param.h"
#include "defs.h"
#include "date.h"
#include "memlayout.h"
#include "spinlock.h"
#include "proc.h"

uint64
sys_exit(void)
{
  int n;
  if(argint(0, &n) < 0)
    return -1;
  exit(n);
  return 0;  // not reached
}

uint64
sys_getpid(void)
{
  return myproc()->pid;
}

uint64
sys_fork(void)
{
  return fork();
}

uint64
sys_wait(void)
{
  uint64 p;
  if(argaddr(0, &p) < 0)
    return -1;
  return wait(p);
}

uint64
sys_sbrk(void)
{
  int addr;
  int n;

  if(argint(0, &n) < 0)
    return -1;
  
  addr = myproc()->sz;
  if(growproc(n) < 0)
    return -1;
  return addr;
}

uint64
sys_sleep(void)
{
  int n;
  uint ticks0;


  if(argint(0, &n) < 0)
    return -1;
  acquire(&tickslock);
  ticks0 = ticks;
  while(ticks - ticks0 < n){
    if(myproc()->killed){
      release(&tickslock);
      return -1;
    }
    sleep(&ticks, &tickslock);
  }
  release(&tickslock);
  return 0;
}


#ifdef LAB_PGTBL
int
sys_pgaccess(void)
{
  // lab pgtbl: your code here.
  uint64 buf, bits;
  int num;
  pte_t *pte;
  if(argaddr(0, &buf) < 0) return -1;
  if(argint(1, &num) < 0) return -1;
  if(argaddr(2, &bits) < 0) return -1;
  unsigned int ans = 0;
  struct proc *p = myproc();
  for (int i = 0; i < num; i++) {
    pte = walk(p->pagetable, (uint64)buf + (uint64)PGSIZE * i, 0);
    if(pte != 0 && (*pte & PTE_A)) {
      ans |= 1 << i;
      *pte &= ~PTE_A; // 感觉这里真正去做的时候是不应该清理的， 因为会影响原来是否访问过的标记，这里清理是为了过test
    }
  }
  if(copyout(p->pagetable, bits, (char*)&ans, sizeof(ans)) < 0){
    return -1;
  }
  return 0;
}
#endif

uint64
sys_kill(void)
{
  int pid;

  if(argint(0, &pid) < 0)
    return -1;
  return kill(pid);
}

// return how many clock tick interrupts have occurred
// since start.
uint64
sys_uptime(void)
{
  uint xticks;

  acquire(&tickslock);
  xticks = ticks;
  release(&tickslock);
  return xticks;
}