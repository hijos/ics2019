#include "cpu/exec.h"

make_EHelper(lui) {
  rtl_sr(id_dest->reg, &id_src->val, 4);

  print_asm_template2(lui);
}

// pa2.1
make_EHelper(auipc){
    s0 = decinfo.seq_pc-4;
    rtl_add(&id_src->val, &s0, &id_src->val);
    rtl_sr(id_dest->reg, &id_src->val, 4);
    print_asm_template2(auipc);
}

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