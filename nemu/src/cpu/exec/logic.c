#include "cpu/exec.h"

make_EHelper(test) {
  TODO();

  print_asm_template2(test);
}

make_EHelper(and) {
  // TODO();
  
  // printf("src1->width = %d\tdest->width = %d\n", id_src->width, id_dest->width);
  // printf("src1 = 0x%08x\tsrc2 = 0x%08x\tdest = 0x%08x\n", id_src->val, id_src2->val, id_dest->val);
  if(id_src->width == 1 && id_dest->width != 1){
	  rtl_sext(&id_src->val, &id_src->val, id_src->width);
	  id_src->width = 4;
  }
  rtl_and(&t0, &id_src->val, &id_dest->val);
  operand_write(id_dest, &t0);
  cpu.OF = 0;
  cpu.CF = 0;
  rtl_update_ZFSF(&t0, id_dest->width);
  // printf("t0 = 0x%08x\tsrc1 = 0x%08x\tsrc2 = 0x%08x\tdest = 0x%08x\n", t0, id_src->val, id_src2->val, id_dest->val);
  print_asm_template2(and);
}

make_EHelper(xor) {
  // TODO();

  // printf("src1 = 0x%08x\tsrc2 = 0x%08x\tdest = 0x%08x\n", id_src->val, id_src2->val, id_dest->val);
  rtl_xor(&t0, &id_src->val, &id_dest->val);
  operand_write(id_dest, &t0);
  cpu.OF = 0;
  cpu.CF = 0;
  rtl_update_ZFSF(&t0, id_dest->width);
  // printf("t0 = 0x%08x\tsrc1 = 0x%08x\tsrc2 = 0x%08x\tdest = 0x%08x\n", t0, id_src->val, id_src2->val, id_dest->val);
  print_asm_template2(xor);
}

make_EHelper(or) {
  // TODO();

  rtl_or(&t0, &id_src->val, &id_dest->val);
  operand_write(id_dest, &t0);
  cpu.OF = 0;
  cpu.CF = 0;
  rtl_update_ZFSF(&t0, id_dest->width);
  print_asm_template2(or);
}

make_EHelper(sar) {
  TODO();
  // unnecessary to update CF and OF in NEMU

  print_asm_template2(sar);
}

make_EHelper(shl) {
  TODO();
  // unnecessary to update CF and OF in NEMU

  print_asm_template2(shl);
}

make_EHelper(shr) {
  TODO();
  // unnecessary to update CF and OF in NEMU

  print_asm_template2(shr);
}

make_EHelper(setcc) {
  uint8_t subcode = decoding.opcode & 0xf;
  printf("eflags = 0x%08x\n", cpu.eflags);
  rtl_setcc(&t2, subcode);
  operand_write(id_dest, &t2);
  print_asm("set%s %s", get_cc_name(subcode), id_dest->str);
}

make_EHelper(not) {
  TODO();

  print_asm_template1(not);
}
