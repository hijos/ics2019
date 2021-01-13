#include "rtl/rtl.h"
#include "cpu/decode.h"

extern DecodeInfo decinfo;

void raise_intr(uint32_t NO, vaddr_t epc) {
  /* TODO: Trigger an interrupt/exception with ``NO''.
   * That is, use ``NO'' to index the IDT.
   */
  decinfo.isa.sepc = epc;
  decinfo.isa.scause = NO;
  decinfo.jmp_pc = decinfo.isa.stvec;
  rtl_j(decinfo.jmp_pc); 
}

bool isa_query_intr(void) {
  return false;
}
