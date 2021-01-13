#include "cpu/exec.h"

make_EHelper(lui) {
  rtl_sr(id_dest->reg, &id_src->val, 4);

  print_asm_template2(lui);
}

// pa2.1
make_EHelper(auipc){
    rtl_add(&id_dest->val, &cpu.pc, &id_src->val);
    rtl_sr(id_dest->reg, &id_dest->val, 4);
    print_asm_template2(auipc);
}

make_EHelper(i) {
   switch(decinfo.isa.instr.funct3){
      case 0:// addi
            rtl_addi(&id_dest->val, &id_src->val, decinfo.isa.instr.simm11_0);
            if (decinfo.isa.instr.rs1){// rs1==0 : li
              if(decinfo.isa.instr.simm11_0)// simm11 == 0 : mv
                print_asm_template3(addi);
              else
                print_asm_template2(mv);              
            } 
            else
              print_asm_template2(li);
            break;
      case 1:// slli
            rtl_shli(&id_dest->val, &id_src->val, decinfo.isa.instr.simm11_0);
            print_asm_template3(slli);
            break;
      case 2:// slti
            id_dest->val = (signed)id_src->val < (signed)decinfo.isa.instr.simm11_0;
            print_asm_template3(slti);
            break;
      case 3:// sltiu
            id_dest->val = (unsigned)id_src->val < (unsigned)decinfo.isa.instr.simm11_0;
            print_asm_template3(sltiu);
            break;
      case 4:// xori
            rtl_xori(&id_dest->val, &id_src->val, decinfo.isa.instr.simm11_0);
            print_asm_template3(xori);
            break;
      case 5:// srli srai
            if(decinfo.isa.instr.funct7){
              rtl_sari(&id_dest->val, &id_src->val, decinfo.isa.instr.rs2);
              print_asm_template3(sari);
            }
            else{
              rtl_shri(&id_dest->val, &id_src->val, decinfo.isa.instr.rs2);
              print_asm_template3(shri);
            }
            break;
      case 6:// ori
            rtl_ori(&id_dest->val, &id_src->val, decinfo.isa.instr.simm11_0);
            print_asm_template3(ori);
            break;
      case 7:// andi
            rtl_andi(&id_dest->val, &id_src->val, decinfo.isa.instr.simm11_0);
            print_asm_template3(andi);
            break;
      default:
            assert(0 && "Unfinished cali opcode");

  }
  rtl_sr(id_dest->reg, &id_dest->val, 4);
}

make_EHelper(r) {
  int funct7 = (unsigned)decinfo.isa.instr.funct7;
  switch(decinfo.isa.instr.funct3){
      case 0:// add sub mul
            if(funct7 & 0b0100000){
              rtl_sub(&id_dest->val, &id_src->val, &id_src2->val);
              print_asm_template3(sub);
            }
            else if(funct7 & 0b1){
              rtl_mul_lo(&id_dest->val, &id_src->val, &id_src2->val);
              print_asm_template3(mul);
            }
            else if(funct7 == 0){
              rtl_add(&id_dest->val, &id_src->val, &id_src2->val);
              print_asm_template3(add);
            }
            else{
              assert(0);
            }
            break;
      case 1:// sll mulh
            if(funct7 & 0b1){
              rtl_imul_hi(&id_dest->val, &id_src->val, &id_src2->val);
              print_asm_template3(mulh);
            }
            else if(funct7 == 0){
              rtl_shl(&id_dest->val, &id_src->val, &id_src2->val);
              print_asm_template3(sll);
            }
            else{
              assert(0);
            }
            break;
      case 2:// slt mulhsu
            if(funct7 & 0b1){
              rtl_mul_hsu(&id_dest->val, &id_src->val, &id_src2->val);
              print_asm_template3(mulhsu);
            }
            else if(funct7 == 0){
              id_dest->val = (signed)id_src->val < (signed)id_src2->val;
              print_asm_template3(slt);
            }
            else{
              assert(0);
            }
            break;
      case 3:// sltu mulhu
            if(funct7 & 0b1){
              rtl_mul_hi(&id_dest->val, &id_src->val, &id_src2->val);
              print_asm_template3(mulhu);
            }
            else if(funct7 == 0){
              id_dest->val = (unsigned)id_src->val < (unsigned)id_src2->val;
              print_asm_template3(sltu);
            }
            else{
              assert(0);
            }
            break;
      case 4:// xor div
            if(funct7 & 0b1){
              rtl_idiv_q(&id_dest->val, &id_src->val, &id_src2->val);
              print_asm_template3(div);
            }
            else if(funct7 == 0){
              rtl_xor(&id_dest->val, &id_src->val, &id_src2->val);
              print_asm_template3(xor);
            }
            else{
              assert(0);
            }
            break;
      case 5:// srl sra divu
            if(decinfo.isa.instr.funct7 & 0x1){
              rtl_idiv_q(&id_dest->val, &id_src->val, &id_src2->val);
              print_asm_template3(divu);
            }
            else if(decinfo.isa.instr.funct7&0b0100000){
              rtl_sar(&id_dest->val, &id_src->val, &id_src2->val);
              print_asm_template3(sar);
            }
            else if(funct7 == 0){
              rtl_shr(&id_dest->val, &id_src->val, &id_src2->val);
              print_asm_template3(shr);
            }
            else{
              assert(0);
            }
            break;
      case 6:// or rem
            if(decinfo.isa.instr.funct7 & 0x1){
              rtl_idiv_r(&id_dest->val, &id_src->val, &id_src2->val);
              print_asm_template3(rem);
            }
            else{
              rtl_or(&id_dest->val, &id_src->val, &id_src2->val);
              print_asm_template3(or);
            }
            break;
      case 7:// and remu
            if(decinfo.isa.instr.funct7){
              rtl_div_r(&id_dest->val, &id_src->val, &id_src2->val);
              print_asm_template3(remu);
            }
            else{
              rtl_and(&id_dest->val, &id_src->val, &id_src2->val);
              print_asm_template3(and);
            }
            break;
      default:
            assert(0 && "Unfinished calr opcode");

  }
  rtl_sr(id_dest->reg, &id_dest->val, 4);
}
