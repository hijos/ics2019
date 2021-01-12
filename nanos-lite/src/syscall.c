#include "common.h"
#include "syscall.h"
#include "fs.h"
#include "proc.h"

extern void naive_uload(PCB *pcb, const char *filename);

static int programBrk;

int do_yield(){
  _yield();
  return 0;
}

int do_execve(const char *fname, char * const argv[], char *const envp[]) {
  naive_uload(NULL, fname);
}

void do_exit(uintptr_t arg){
  //_halt(arg);
  do_execve("/bin/init", NULL, NULL);
}

int do_write(int fd, const void*buf, size_t count){
  // if(fd==1 || fd==2){
  //   for(int i = 0;i < count;i++){
  //       _putc(((char*)buf)[i]);
  //   }
  //   return count;
  // }
  // return 0;
  // Log("into do_write");
  return fs_write(fd, buf, count);
}

int do_open(const char*path, int flags, int mode){
  return fs_open(path, flags, mode);
}

int do_close(int fd){
  return fs_close(fd);
}

int do_read(int fd, void*buf, size_t count){
  // Log("into do_read");
  return fs_read(fd, buf, count);
}

size_t do_lseek(int fd, size_t offset, int whence){
  return fs_lseek(fd, offset, whence);
}

int do_brk(int addr){
  programBrk = addr;
  return 0;
}

_Context* do_syscall(_Context *c) {
  uintptr_t a[4];
  a[0] = c->GPR1;
  a[1] = c->GPR2;
  a[2] = c->GPR3;
  a[3] = c->GPR4;

  switch (a[0]) {
    case SYS_yield:
        c->GPRx = do_yield();
        break;
    case SYS_exit:
        do_exit(a[1]);
        break;
    case SYS_write:
        c->GPRx = do_write(a[1], (void*)(a[2]), a[3]);
        break;
    case SYS_read:
        c->GPRx = do_read(a[1], (void*)(a[2]), a[3]);
        break;
    case SYS_lseek:
        c->GPRx = do_lseek(a[1], a[2], a[3]);
        break;
    case SYS_open:
        c->GPRx = do_open((const char *)a[1], a[2], a[3]);
        break;
    case SYS_close:
        c->GPRx = do_close(a[1]);
        break;
    case SYS_brk:
        c->GPRx = do_brk(a[1]);
        break;
    case SYS_execve:
        c->GPRx = do_execve(a[1], a[2], a[3]);
        break;
    default: panic("Unhandled syscall ID = %d", a[0]);
  }

  return NULL;
}
