#include <common.h>
#include "syscall.h"

int _sys_write(int fd, void *buf, size_t count){
    if(fd!=1&&fd!=2){
        return -1;
    }
    printf("write %d\n",count);
    char *str=(char *)buf;
    for(int i=0;i<count;i++){
        putch(str[i]);
    }
    return count;
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
        case SYS_write:c->GPRx=_sys_write(a[1],(void*)a[2],a[3]);break;
        case SYS_brk:c->GPRx=0;break;
        default: panic("Unhandled syscall ID = %d", a[0]);
    }
}
