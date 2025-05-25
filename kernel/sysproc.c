#include "types.h"
#include "riscv.h"
#include "defs.h"
#include "param.h"
#include "memlayout.h"
#include "spinlock.h"
#include "proc.h"
#include "sysinfo.h"

// Deing used to declare the explicit existence of kfreemem() and count_active_procs()
extern uint64 kfreemem(void);
extern int count_active_procs(void);

uint64
sys_sysinfo(void)
{
  struct sysinfo backup;
  struct proc *p = myproc();
  uint64 address; // &info that we passed in when calling sysinfo in sysinfotest.c

  argaddr(0, &address); // get &info from a0 register
  
  backup.freemem = kfreemem(); // return the number of bytes of free memory in kernel
  backup.nproc = count_active_procs(); // return the number of processs whose state is UNUSED

  if (copyout(p->pagetable, address, (char *)&backup, sizeof(backup)) < 0) // copy the data from backup to address(&info) by using current process's pagetable
    return -1;

  return 0;
}

uint64
sys_trace(void)
{
  int mask;
  argint(0, &mask);
  myproc()->mask = mask;
  return 0;
}

uint64
sys_exit(void)
{
  int n;
  argint(0, &n);
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
  argaddr(0, &p);
  return wait(p);
}

uint64
sys_sbrk(void)
{
  uint64 addr;
  int n;

  argint(0, &n);
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

  argint(0, &n);
  if(n < 0)
    n = 0;
  acquire(&tickslock);
  ticks0 = ticks;
  while(ticks - ticks0 < n){
    if(killed(myproc())){
      release(&tickslock);
      return -1;
    }
    sleep(&ticks, &tickslock);
  }
  release(&tickslock);
  return 0;
}

uint64
sys_kill(void)
{
  int pid;

  argint(0, &pid);
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
