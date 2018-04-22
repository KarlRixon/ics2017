#include "cpu/exec.h"

void diff_test_skip_qemu();
void diff_test_skip_nemu();

make_EHelper(lidt) {
  TODO();

  print_asm_template1(lidt);
}

make_EHelper(mov_r2cr) {
  TODO();

  print_asm("movl %%%s,%%cr%d", reg_name(id_src->reg, 4), id_dest->reg);
}

make_EHelper(mov_cr2r) {
  TODO();

  print_asm("movl %%cr%d,%%%s", id_src->reg, reg_name(id_dest->reg, 4));

#ifdef DIFF_TEST
  diff_test_skip_qemu();
#endif
}

make_EHelper(int) {
  TODO();

  print_asm("int %s", id_dest->str);

#ifdef DIFF_TEST
  diff_test_skip_nemu();
#endif
}

make_EHelper(iret) {
  TODO();

  print_asm("iret");
}

uint32_t pio_read(ioaddr_t, int);
void pio_write(ioaddr_t, int, uint32_t);

make_EHelper(in) {
  // TODO();
  // printf("src = 0x%08x\tdest = 0x%08X\n", id_src->val, id_dest->val);
  t0 = pio_read(id_src->val, id_dest->width);
  // printf("t0 = 0x%08x\n", t0);
  rtl_mv(&id_dest->val, &t0);
  // printf("dest = 0x%08x\n", id_dest->val);
  print_asm_template2(in);

#ifdef DIFF_TEST
  diff_test_skip_qemu();
#endif
}

make_EHelper(out) {
  // TODO();
  // printf("src = 0x%08x\tdest = 0x%08X\n", id_src->val, id_dest->val);
  // printf("src-width = %d\tdest-width = %d\n", id_src->width, id_dest->width);
  t0 = pio_read(id_src->val, id_dest->width);
   printf("t0 = 0x%08x\n", t0);
  pio_write(id_dest->val, id_src->width, id_src->val);
  t0 = pio_read(id_src->val, id_dest->width);
   printf("t0 = 0x%08x\n", t0);
  print_asm_template2(out);

#ifdef DIFF_TEST
  diff_test_skip_qemu();
#endif
}
