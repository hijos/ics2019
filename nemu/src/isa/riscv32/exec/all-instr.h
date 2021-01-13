#include "cpu/exec.h"

make_EHelper(lui);

make_EHelper(ld);
make_EHelper(st);

make_EHelper(inv);
make_EHelper(nemu_trap);

// pa2.1
make_EHelper(auipc);
make_EHelper(jal);
make_EHelper(jalr);
make_EHelper(i);
make_EHelper(r);

// pa2.2
make_EHelper(branch);

// pa3.1
make_EHelper(system);
