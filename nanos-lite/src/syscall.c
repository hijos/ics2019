#include "common.h"
#include "syscall.h"

int do_write(int fd, const void*buf, size_t count){
  if(fd==1 || fd==2){
    for(int i = 0;i < count;i++){
        _putc(((char*)buf)[i]);
    }
    return count;
  }
  return 0;
  // int res = fs_write(fd, buf, count);
  // return res;
}

_Context* do_syscall(_Context *c) {
  uintptr_t a[4];
  a[0] = c->GPR1;
  a[1] = c->GPR2;
  a[2] = c->GPR3;
  a[3] = c->GPR4;

  switch (a[0]) {
    case SYS_yield:
        _yield();
        c->GPRx = 0;
        break;
    case SYS_exit:
        // naive_uload(NULL,"/bin/init");
        _halt(a[1]);
        break;
    case SYS_write:
        c->GPRx = do_write(a[1], (void*)(a[2]), a[3]);
        break;
    default: panic("Unhandled syscall ID = %d", a[0]);
  }

  return NULL;
}
