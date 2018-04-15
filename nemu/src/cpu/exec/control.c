#include "cpu/exec.h"

make_EHelper(jmp) {
  // the target address is calculated at the decode stage
  decoding.is_jmp = 1;

  print_asm("jmp %x", decoding.jmp_eip);
}

make_EHelper(jcc) {
  // the target address is calculated at the decode stage
  uint8_t subcode = decoding.opcode & 0xf;
  rtl_setcc(&t2, subcode);
  rtl_sext(&id_dest->val, &id_dest->val, 4);
  decoding.jmp_eip = decoding.seq_eip + id_dest->val;
  // if(id_dest->width == 1) decoding.jmp_eip -= 0x100;
  // if(id_dest->width == 2) decoding.jmp_eip -= 0x10000;
  // printf("dest->width = %x\n", id_dest->width);
  // printf("seq_eip = 0x%08x\tdest_.val = 0x%08x\tjmp_eip = 0x%08x\n", decoding.seq_eip, id_dest->val, decoding.jmp_eip);
  decoding.is_jmp = t2;

  print_asm("j%s %x", get_cc_name(subcode), decoding.jmp_eip);
}

make_EHelper(jmp_rm) {
  decoding.jmp_eip = id_dest->val;
  decoding.is_jmp = 1;

  print_asm("jmp *%s", id_dest->str);
}

make_EHelper(call) {
  // the target address is calculated at the decode stage
  // TODO();

  rtl_push(&decoding.seq_eip, id_dest->width);
  decoding.is_jmp = 1;
  decoding.jmp_eip = decoding.seq_eip + id_dest->val;

  print_asm("call %x", decoding.jmp_eip);
}

make_EHelper(ret) {
  // TODO();

  // printf("src1 = 0x%08x\tsrc2 = 0x%08x\tdest = 0x%08x\n)", id_src->val, id_src2->val, id_dest->val);
  rtl_pop(&t0);
  // printf("t0 = 0x%08x\n", t0);
  decoding.is_jmp = 1;
  decoding.jmp_eip = t0;

  print_asm("ret");
}

make_EHelper(call_rm) {
  TODO();

  print_asm("call *%s", id_dest->str);
}
