#include <common.h>
#include "syscall.h"
#include <proc.h>
#define time_t uint64_t
#define suseconds_t uint64_t
struct timeval {
    time_t      tv_sec;     /* seconds */
    suseconds_t tv_usec;    /* microseconds */
};
extern void naive_uload(PCB *pcb, const char *filename);
static uintptr_t sys_gettimeofday(uintptr_t *args) {
  struct timeval *tv = (void *)args[1];
  //struct timezone *tz = (void *)args[1];
  uint64_t us=io_read(AM_TIMER_UPTIME).us;
  if(tv!=NULL){
      tv->tv_sec=us/1000000;
      tv->tv_usec=us%1000000;
  }
  return 0;
}
void do_syscall(Context *c) {
    uintptr_t a[4];
    a[0] = c->GPR1;
    a[1] = c->GPR2;
    a[2] = c->GPR3;
    a[3] = c->GPR4;
    switch (a[0]) {
        case SYS_yield: yield(); c->GPRx = 0; break;
        case SYS_exit: halt(c->GPR2); break;
        case SYS_write:c->GPRx=fs_write(a[1],(void*)a[2],a[3]);break;
        case SYS_brk:c->GPRx=0;break;
        case SYS_open:c->GPRx=fs_open((char*)a[1],a[2],a[3]);break;
        case SYS_read:c->GPRx=fs_read(a[1],(void*)a[2],a[3]);break;
        case SYS_close:c->GPRx=fs_close(a[1]);break;
        case SYS_lseek:c->GPRx=fs_lseek(a[1],a[2],a[3]);break;
        case SYS_gettimeofday:c->GPRx=sys_gettimeofday(a);break;
        case SYS_execve: naive_uload(NULL,(char*)a[1]);c->GPRx=-1;break;
        default: panic("Unhandled syscall ID = %d", a[0]);
    }
}

