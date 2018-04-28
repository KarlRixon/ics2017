#include "cpu/exec.h"

make_EHelper(mov) {
  operand_write(id_dest, &id_src->val);
  print_asm_template2(mov);
}

make_EHelper(push) {
  // TODO();

  // printf("src1 = 0x%08x\tsrc2 = 0x%08x\tdest = 0x%08x\n", id_src->val, id_src2->val, id_dest->val);
  // printf("id_dest->width = %d\tid_dest->str = %s\n", id_dest->width, id_dest->str);
  rtl_sext(&t2, &id_dest->val, id_dest->width);
  rtl_push(&t2, id_dest->width);

  print_asm_template1(push);
}

make_EHelper(pop) {
  // TODO();

  // printf("t2 = %d | 0x%x\n", t2, t2);
  rtl_pop(&t2);
  operand_write(id_dest, &t2);
  // printf("t2 = %d | 0x%x\n", t2, t2);

  print_asm_template1(pop);
}

make_EHelper(pusha) {
  // TODO();
  t0 = cpu.esp;
  rtl_push(&cpu.eax, 4);
  rtl_push(&cpu.ecx, 4);
  rtl_push(&cpu.edx, 4);
  rtl_push(&cpu.ebx, 4);
  rtl_push(&t0, 4);
  rtl_push(&cpu.ebp, 4);
  rtl_push(&cpu.esi, 4);
  rtl_push(&cpu.edi, 4);
  print_asm("pusha");
}

make_EHelper(popa) {
  // TODO();
  rtl_pop(&cpu.edi);
  rtl_pop(&cpu.esi);
  rtl_pop(&cpu.ebp);
  rtl_pop(&t0);
  rtl_pop(&cpu.ebx);
  rtl_pop(&cpu.edx);
  rtl_pop(&cpu.ecx);
  rtl_pop(&cpu.eax);
  print_asm("popa");
}

make_EHelper(leave) {
  // TODO();
  rtl_mv(&cpu.esp, &cpu.ebp);
  rtl_sr(R_EBP, id_dest->width, guest_to_host(cpu.esp));
  rtl_addi(&cpu.esp, &cpu.esp, id_src->width);
  print_asm("leave");
}

make_EHelper(cltd) {
  rtl_lr(&t1, R_EAX, id_dest->width);
  rtl_slt(&t0, &t1, &tzero);
  if (t0 == 1)
	t2 = 0xffffffff;
  else
	t2 = 0;
  rtl_sr(R_EDX, id_dest->width, &t2);

  print_asm(decoding.is_operand_size_16 ? "cwtl" : "cltd");
}

make_EHelper(cwtl) {
  if (decoding.is_operand_size_16) {
    // TODO();
	rtl_lr(&t0, R_AL, 1);
	t0 = (int16_t)(int8_t)(uint8_t)t0;
	rtl_sr(R_AX, 2, &t0);
  }
  else {
    // TODO();
	rtl_lr(&t0, R_AX, 2);
    t0 = (int32_t)(int16_t)(uint16_t)t0;
	rtl_sr(R_EAX, 4, &t0);
  }

  print_asm(decoding.is_operand_size_16 ? "cbtw" : "cwtl");
}

make_EHelper(movsx) {
  id_dest->width = decoding.is_operand_size_16 ? 2 : 4;
  rtl_sext(&t2, &id_src->val, id_src->width);
  operand_write(id_dest, &t2);
  print_asm_template2(movsx);
}

make_EHelper(movzx) {
  id_dest->width = decoding.is_operand_size_16 ? 2 : 4;
  // printf("src1->width = %d\tsrc2->width = %d\tdest->width = %d\n", id_src->width, id_src2->width, id_dest->width);
  // printf("src1->str = %s\tsrc2->str = %s\tdest->str = %s\n", id_src->str, id_src2->str, id_dest->str);
  // printf("src1 = 0x%08x\tsrc2 = 0x%08x\tdest = 0x%08x\n", id_src->val, id_src2->val, id_dest->val);
  
  operand_write(id_dest, &id_src->val);
  print_asm_template2(movzx);
}

make_EHelper(lea) {
  rtl_li(&t2, id_src->addr);
  operand_write(id_dest, &t2);
  print_asm_template2(lea);
}
