#include "proc.h"
#include <elf.h>
#include "fs.h"

#ifdef __ISA_AM_NATIVE__
# define Elf_Ehdr Elf64_Ehdr
# define Elf_Phdr Elf64_Phdr
#else
# define Elf_Ehdr Elf32_Ehdr
# define Elf_Phdr Elf32_Phdr
#endif

extern size_t ramdisk_read(void *, size_t, size_t);

static uintptr_t loader(PCB *pcb, const char *filename) {
  // TODO();
  Elf_Ehdr Ehdr;// read the ELF Header
  ramdisk_read(&Ehdr, 0, sizeof(Ehdr));
  Elf_Phdr Phdr;//read Program Header
  ramdisk_read(&Phdr, Ehdr.e_phoff, sizeof(Phdr));
  ramdisk_read((void*)Phdr.p_vaddr, Phdr.p_offset, Phdr.p_filesz);
  memset((void*)(Phdr.p_vaddr+Phdr.p_filesz),0,(Phdr.p_memsz-Phdr.p_filesz));

  return Ehdr.e_entry;

  // Elf_Ehdr Ehdr;
  // Elf_Phdr Phdr;
  // int fd = fs_open(filename,0,0);
  // assert(fd!=-1);
  // fs_read(fd,&Ehdr,sizeof(Elf_Ehdr));
  // fs_lseek(fd,Ehdr.e_phoff,SEEK_SET);
  // size_t open_offset;

  // for (uint16_t i=0; i<Ehdr.e_phnum; i++){

  //   fs_read(fd,&Phdr,sizeof(Elf_Phdr));
  //   open_offset = fs_open_offset(fd);
  //   if(Phdr.p_type == PT_LOAD){
  //     fs_lseek(fd,Phdr.p_offset,SEEK_SET);
  //     fs_read(fd,(void *)Phdr.p_vaddr,Phdr.p_filesz);
  //     memset((void*)(Phdr.p_vaddr+Phdr.p_filesz),0,(Phdr.p_memsz-Phdr.p_filesz));
  //   }
  //   fs_lseek(fd,open_offset,SEEK_SET);

  // }
  // fs_close(fd);
  // return Ehdr.e_entry;
}

void naive_uload(PCB *pcb, const char *filename) {
  uintptr_t entry = loader(pcb, filename);
  Log("Jump to entry = %x", entry);
  ((void(*)())entry) ();
}

void context_kload(PCB *pcb, void *entry) {
  _Area stack;
  stack.start = pcb->stack;
  stack.end = stack.start + sizeof(pcb->stack);

  pcb->cp = _kcontext(stack, entry, NULL);
}

void context_uload(PCB *pcb, const char *filename) {
  uintptr_t entry = loader(pcb, filename);

  _Area stack;
  stack.start = pcb->stack;
  stack.end = stack.start + sizeof(pcb->stack);

  pcb->cp = _ucontext(&pcb->as, stack, stack, (void *)entry, NULL);
}
