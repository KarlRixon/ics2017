#include "cpu/exec.h"

make_EHelper(test) {
  // TODO();

  // printf("src1->width = %dsrc1->type = %dtdest->width = %ddest->type = %d\n", id_src->width, id_src->type, id_dest->width, id_dest->type);
  // printf("src1 = 0x%08xsrc1->reg = 0x%08x\tsrc2 = 0x%08x\tdest = 0x%08x\tdest->reg = 0x%08x\tdest->str = %s\n", id_src->val, id_src->reg, id_src2->val, id_dest->val,id_dest->reg, id_dest->str);
  
  rtl_sext(&id_src->val, &id_src->val, id_src->width);
  rtl_sext(&id_dest->val, &id_dest->val, id_dest->width);
  // printf("src1 = 0x%08xsrc1->reg = 0x%08x\tsrc2 = 0x%08x\tdest = 0x%08x\tdest->reg = 0x%08x\tdest->str = %s\n", id_src->val, id_src->reg, id_src2->val, id_dest->val,id_dest->reg, id_dest->str);
  
  rtl_and(&t0, &id_dest->val, &id_src->val);
  rtl_set_OF(&tzero);
  rtl_set_CF(&tzero);
  // printf("t0 = %d\n", t0);
  rtl_update_ZFSF(&t0, id_dest->width);
  // printf("SF = %d\n", cpu.SF);

  print_asm_template2(test);
}

make_EHelper(and) {
  // TODO();
  
  // printf("src1->width = %d\tdest->width = %d\n", id_src->width, id_dest->width);
  // printf("src1 = 0x%08x\tsrc2 = 0x%08x\tdest = 0x%08x\n", id_src->val, id_src2->val, id_dest->val);
  rtl_sext(&id_src->val, &id_src->val, id_src->width);
  rtl_sext(&id_dest->val, &id_dest->val, id_dest->width);
  
  rtl_and(&t0, &id_dest->val, &id_src->val);
  operand_write(id_dest, &t0);
  rtl_set_OF(&tzero);
  rtl_set_CF(&tzero);
  rtl_update_ZFSF(&t0, id_dest->width);
  // printf("t0 = 0x%08x\tsrc1 = 0x%08x\tsrc2 = 0x%08x\tdest = 0x%08x\n", t0, id_src->val, id_src2->val, id_dest->val);
  print_asm_template2(and);
}

make_EHelper(xor) {
  // TODO();

  // printf("src1 = 0x%08x\tsrc2 = 0x%08x\tdest = 0x%08x\n", id_src->val, id_src2->val, id_dest->val);
  rtl_sext(&id_src->val, &id_src->val, id_src->width);
  rtl_sext(&id_dest->val, &id_dest->val, id_dest->width);
  rtl_xor(&t0, &id_src->val, &id_dest->val);
  operand_write(id_dest, &t0);
  rtl_set_OF(&tzero);
  rtl_set_CF(&tzero);
  rtl_update_ZFSF(&t0, id_dest->width);
  // printf("t0 = 0x%08x\tsrc1 = 0x%08x\tsrc2 = 0x%08x\tdest = 0x%08x\n", t0, id_src->val, id_src2->val, id_dest->val);
  print_asm_template2(xor);
}

make_EHelper(or) {
  // TODO();
  rtl_sext(&id_src->val, &id_src->val, id_src->width);
  rtl_sext(&id_dest->val, &id_dest->val, id_dest->width);

  rtl_or(&t0, &id_src->val, &id_dest->val);
  operand_write(id_dest, &t0);
  rtl_set_OF(&tzero);
  rtl_set_CF(&tzero);
  rtl_update_ZFSF(&t0, id_dest->width);
  print_asm_template2(or);
}

make_EHelper(sar) {
  // TODO();
  // unnecessary to update CF and OF in NEMU
  // printf("src1->width = %d\tdest->width = %ddest->type = %d\n", id_src->width, id_dest->width, id_dest->type);
  // printf("src1 = 0x%08x\tsrc2 = 0x%08x\tdest->reg = 0x%08x\n", id_src->val, id_src2->val, id_dest->reg);
  rtl_sext(&id_dest->val, &id_dest->val, id_dest->width);
  rtl_sar(&t0, &id_dest->val, &id_src->val);
  operand_write(id_dest, &t0);
  // if(t0 == 1) rtl_set_OF(&tzero);
  print_asm_template2(sar);
}

make_EHelper(shl) {
  // TODO();
  // unnecessary to update CF and OF in NEMU
  // printf("src1->str = %s\tsrc2->str = %sdest->str = %s\n", id_src->str, id_src2->str, id_dest->str);
  // printf("src1->reg = 0x%08x\tsrc2 = 0x%08x\tdest->val = 0x%08x\n", id_src->reg, id_src2->val, id_dest->val);
  // rtl_lr(&t0, id_src->reg, id_src->width);
  rtl_sext(&id_dest->val, &id_dest->val, id_dest->width);
  rtl_shl(&t0, &id_dest->val, &id_src->val);
  // printf("id_src->regval = 0x%08x\n", t0);
  operand_write(id_dest, &t0);
  print_asm_template2(shl);
}

make_EHelper(shr) {
  // TODO();
  // unnecessary to update CF and OF in NEMU
  rtl_shr(&t0, &id_dest->val, &id_src->val);
  operand_write(id_dest, &t0);

  print_asm_template2(shr);
}

make_EHelper(setcc) {
	uint8_t subcode = decoding.opcode & 0xf;
	// printf("opcode = 0x%0x", decoding.opcode);
	// printf("eflags = 0x%08x\n", cpu.eflags);
	// printf("src1->width = %d\tdest->width = %d\tsrc->type = %d\tdest->type = %d\n", id_src->width, id_dest->width, id_src->type, id_dest->type);
	rtl_setcc(&t2, subcode);
	
	// printf("t0 = 0x%08x\tt2 = 0x%80x\tsrc1 = 0x%08x\tsrc2 = 0x%08x\tdest = 0x%08x\n", t0, t2, id_src->val, id_src2->val, id_dest->val);
	operand_write(id_dest, &t2);
	// printf("t0 = 0x%08x\tt2 = 0x%80x\tsrc1 = 0x%08x\tsrc2 = 0x%08x\tdest = 0x%08x\n", t0, t2, id_src->val, id_src2->val, id_dest->val);
	print_asm("set%s %s", get_cc_name(subcode), id_dest->str);
}

make_EHelper(not) {
  // TODO();
  rtl_sext(&id_dest->val, &id_dest->val, id_dest->width);

  rtl_not(&id_dest->val);
  operand_write(id_dest, &id_dest->val);
  print_asm_template1(not);
}
