#include "cpu/exec.h"

make_EHelper(jal){
    s0 = decinfo.seq_pc;
    s1 = 4;
    rtl_sr(id_dest->reg, &s0, 4);
    rtl_sub(&s0, &s0, &s1);
    rtl_add(&(decinfo.jmp_pc), &s0, &id_src->val);
    decinfo_set_jmp(true);
    print_asm_template2(jal);
}

make_EHelper(jalr){
    s0 = decinfo.seq_pc;
    rtl_sr(id_dest->reg, &s0, 4);
    decinfo.jmp_pc = (id_src->val+id_src2->val)&~1;
    decinfo_set_jmp(true);
    print_asm_template2(jalr);
}
