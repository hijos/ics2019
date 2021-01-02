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

make_EHelper(cali) {
   switch(decinfo.isa.instr.funct3){
      case 0:// addi
            rtl_addi(&id_dest->val, &id_src->val, decinfo.isa.instr.simm11_0);
            print_asm_template2(addi);
            break;
      case 1:// slli
            rtl_shli(&id_dest->val, &id_src->val, decinfo.isa.instr.simm11_0);
            print_asm_template2(slli);
            break;
      case 2:// slti
            id_dest->val = (signed)id_src->val < (signed)decinfo.isa.instr.simm11_0;
            print_asm_template2(slti);
            break;
      case 3:// sltiu
            id_dest->val = (unsigned)id_src->val < (unsigned)decinfo.isa.instr.simm11_0;
            print_asm_template2(sltiu);
            break;
      case 4:// xori
            rtl_xori(&id_dest->val, &id_src->val, decinfo.isa.instr.simm11_0);
            print_asm_template2(xori);
            break;
      case 5:// srli srai
            if(decinfo.isa.instr.funct7){
              rtl_sari(&id_dest->val, &id_src->val, decinfo.isa.instr.rs2);
              print_asm_template2(sari);
            }
            else{
              rtl_shri(&id_dest->val, &id_src->val, decinfo.isa.instr.rs2);
              print_asm_template2(shri);
            }
            break;
      case 6:// ori
            rtl_ori(&id_dest->val, &id_src->val, decinfo.isa.instr.simm11_0);
            print_asm_template2(ori);
            break;
      case 7:// andi
            rtl_andi(&id_dest->val, &id_src->val, decinfo.isa.instr.simm11_0);
            print_asm_template2(andi);
            break;
      default:
            assert(0 && "Unfinished CALI opcode");

  }
  rtl_sr(id_dest->reg, &id_dest->val, 4);
}

make_EHelper(calr) {
  int funct7 = (unsigned)decinfo.isa.instr.funct7;
  switch(decinfo.isa.instr.funct3){
      case 0:// add sub 
            if(funct7 != 0){
              rtl_sub(&id_dest->val, &id_src->val, &id_src2->val);
              print_asm_template2(sub);
            }
            else{
              rtl_add(&id_dest->val, &id_src->val, &id_src2->val);
              print_asm_template2(add);
            }
            break;
      case 1:// sll
            rtl_shl(&id_dest->val, &id_src->val, &id_src2->val);
            print_asm_template2(sll);
            break;
      case 2:// slt
            id_dest->val = (signed)id_src->val < (signed)id_src2->val;
            print_asm_template2(slt);
            break;
      case 3:// sltu
            id_dest->val = (unsigned)id_src->val < (unsigned)id_src2->val;
            print_asm_template2(sltu);
            break;
      case 4:// xor
            rtl_xor(&id_dest->val, &id_src->val, &id_src2->val);
            print_asm_template2(xor);
            break;
      case 5:// srl sra
            if(funct7 != 0){
              rtl_sar(&id_dest->val, &id_src->val, &id_src2->val);
              print_asm_template2(sar);
            }
            else {
              rtl_shr(&id_dest->val, &id_src->val, &id_src2->val);
              print_asm_template2(shr);
            }
            break;
      case 6:// or
            rtl_or(&id_dest->val, &id_src->val, &id_src2->val);
            print_asm_template2(or);
            break;
      case 7:// and
            rtl_and(&id_dest->val, &id_src->val, &id_src2->val);
            print_asm_template2(and);
            break;
      default:
            assert(0 && "Unfinished CALU opcode");

  }
  rtl_sr(id_dest->reg, &id_dest->val, 4);
}

/*
make_EHelper(calr) {
  int funct7 = (unsigned)decinfo.isa.instr.funct7;
  switch(decinfo.isa.instr.funct3){
      case 0:// add sub 
          if(funct7 & 0b0100000){
            rtl_sub(&id_dest->val, &id_src->val, &id_src2->val);
            print_asm_template2(sub);
          }
          else if(funct7 & 0b1){
            rtl_mul_lo(&id_dest->val, &id_src->val, &id_src2->val);
            print_asm_template2(mul);
          }
          else if(funct7 == 0){
            rtl_add(&id_dest->val, &id_src->val, &id_src2->val);
            print_asm_template2(add);
          }
          else{
              assert(0);
          }
          break;
          //TODO: remaining thing may need to fill
      case 1:// sll
          if(funct7 & 0b1){
            rtl_imul_hi(&id_dest->val, &id_src->val, &id_src2->val);
            print_asm_template2(mulh);
          }
          else if(funct7 == 0){
            rtl_shl(&id_dest->val, &id_src->val, &id_src2->val);
            print_asm_template2(sll);
          }
          else{
              assert(0);
          }
            break;
      case 2:// slt
            id_dest->val = (signed)id_src->val < (signed)id_src2->val;
            print_asm_template2(slt);
            break;
      case 3:// sltu
          if(funct7 & 0b1){
            rtl_mul_hi(&id_dest->val, &id_src->val, &id_src2->val);
            print_asm_template2(mulhu);
          }
          else if(funct7 == 0){
            id_dest->val = (unsigned)id_src->val < (unsigned)id_src2->val;
            print_asm_template2(sltu);
          }
          else{
              assert(0);
          }
            break;
      case 4:// xor
          if(funct7 & 0b1){
            rtl_idiv_q(&id_dest->val, &id_src->val, &id_src2->val);
            print_asm_template2(div);
          }
          else if(funct7 == 0){
            rtl_xor(&id_dest->val, &id_src->val, &id_src2->val);
            print_asm_template2(xor);
          }
          else{
              assert(0);
          }
            break;
      case 5:// srl sra
          if(decinfo.isa.instr.funct7 & 0x1){
            rtl_idiv_q(&id_dest->val, &id_src->val, &id_src2->val);
            print_asm_template2(divu);
          }
          else if(decinfo.isa.instr.funct7&0b0100000){
            rtl_sar(&id_dest->val, &id_src->val, &id_src2->val);
            print_asm_template2(sar);
          }
          else if(funct7 == 0){
            rtl_shr(&id_dest->val, &id_src->val, &id_src2->val);
            print_asm_template2(shr);
          }
          else{
              assert(0);
          }
            break;
      case 6:// or
          if(decinfo.isa.instr.funct7 & 0x1){
            rtl_idiv_r(&id_dest->val, &id_src->val, &id_src2->val);
            print_asm_template2(rem);
          }
          else{
            rtl_or(&id_dest->val, &id_src->val, &id_src2->val);
            print_asm_template2(or);
          }
            break;
      case 7:// and
          if(decinfo.isa.instr.funct7){
            rtl_div_r(&id_dest->val, &id_src->val, &id_src2->val);
            print_asm_template2(remu);
          }
          else{
            rtl_and(&id_dest->val, &id_src->val, &id_src2->val);
            print_asm_template2(and);
          }
            break;
      default:
            assert(0 && "Unfinished CALU opcode");

  }
  rtl_sr(id_dest->reg, &id_dest->val, 4);

}*/