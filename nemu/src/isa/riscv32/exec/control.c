#include "cpu/exec.h"

make_EHelper(jal){
    s0 = cpu.pc + 4;
    rtl_sr(id_dest->reg, &s0, 4);
    rtl_add(&(decinfo.jmp_pc), &cpu.pc, &id_src->val);
    rtl_j(decinfo.jmp_pc); 
    print_asm_template2(jal);
}

make_EHelper(jalr){
    s0 = cpu.pc + 4;
    rtl_sr(id_dest->reg, &s0, 4);
    rtl_add(&decinfo.jmp_pc, &id_src->val, &id_src2->val);
    rtl_j(decinfo.jmp_pc);
    print_asm_template2(jalr);
}
