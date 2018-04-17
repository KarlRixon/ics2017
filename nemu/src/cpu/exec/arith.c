#include "cpu/exec.h"

make_EHelper(add) {
  // TODO();

  // printf("src->width = %d\tsrc2->width = %d\tdest->width = %d\n", id_src->width, id_src2->width, id_dest->width);
  // printf("src->val = 0x%08x\tsrc2->val = 0x%08x\tdest->val = 0x%08x\n", id_src->val, id_src2->val, id_dest->val);
  if(id_src->width == 1 && id_dest->width != 1){
	  rtl_sext(&id_src->val, &id_src->val, id_src->width);
	  id_src->width = id_dest->width;
  }
  rtl_add(&t0, &id_dest->val, &id_src->val);
  if((id_src->val > 0 && id_dest->val > 0 && t0 < 0) || (id_src->val < 0 && id_dest->val < 0 && t0 > 0)) cpu.OF = 1;
  else cpu.OF = 0;
  cpu.CF = t0 < id_dest->val?1:0;
  operand_write(id_dest, &t0);
  rtl_update_ZFSF(&t0, id_dest->width);

  print_asm_template2(add);
}

make_EHelper(sub) {
  // TODO();

  // printf("src->val = 0x%08x\tsrc->width = %d\n", id_src->val, id_src->width);
  if(id_src->width == 1 && id_dest->width != 1){
	  // id_src->val = 0xf8;
	  rtl_sext(&id_src->val, &id_src->val, id_src->width);
	  id_src->width = id_dest->width;
  }
  rtl_sub(&t0, &id_dest->val, &id_src->val);
  if((id_src->val < 0 && id_dest->val > 0 && t0 < 0) || (id_src->val < 0 && id_dest->val > 0 && t0 > 0)) cpu.OF = 1;
  else cpu.OF = 0;
  cpu.CF = t0 > id_dest->val?1:0;
  operand_write(id_dest, &t0);
  rtl_update_ZFSF(&t0, id_dest->width);
  // printf("src->val = 0x%08x\tsrc->width = %d\n", id_src->val, id_src->width);

  print_asm_template2(sub);
}

make_EHelper(cmp) {
	// TODO();

	// printf("src->type = %d\tsrc2->type = %d\tdest->type = %d\n", id_src->type, id_src2->type, id_dest->type);
	// printf("src->width = %d\tsrc2->width = %d\tdest->width = %d\n", id_src->width, id_src2->width, id_dest->width);
	// printf("src->val = 0x%08x\tsrc2->val = 0x%08x\tdest->val = 0x%08x\n", id_src->val, id_src2->val, id_dest->val);
	if(id_src->width == 1 && id_dest->width != 1){
		rtl_sext(&id_src->val, &id_src->val, id_src->width);
		id_src->width = id_dest->width;
	}
	rtl_sub(&t0, &id_dest->val, &id_src->val);
	// printf("t0 = 0x%08x\tsrc->val = 0x%08x\tsrc2->val = 0x%08x\tdest->val = 0x%08x\n", t0, id_src->val, id_src2->val, id_dest->val);
	if((id_src->val <= 0 && id_dest->val < 0 && t0 < 0) || (id_src->val < 0 && id_dest->val >= 0 && t0 >= 0)) cpu.OF = 1;
	else cpu.OF = 0;
	cpu.CF = t0 > id_dest->val?1:0;
	rtl_update_ZFSF(&t0, id_dest->width);
	// printf("cpu.ZF = %d\n", cpu.ZF);
	print_asm_template2(cmp);
}

make_EHelper(inc) {
  TODO();

  print_asm_template1(inc);
}

make_EHelper(dec) {
  // TODO();
  rtl_sext(&id_dest->val, &id_dest->val, id_dest->width);
  rtl_subi(&t0, &id_dest->val, 0x1);
  operand_write(id_dest, &t0);
  rtl_update_ZFSF(&t0, id_dest->width);
  
  rtl_xor(&t1, &id_dest->val, &id_src->val);
  rtl_xor(&t2, &t0, &id_dest->val);
  rtl_and(&t1, &t1, &t2);
  rtl_msb(&t1, &t1, id_dest->width);
  rtl_set_OF(&t1);
  print_asm_template1(dec);
}

make_EHelper(neg) {
  TODO();

  print_asm_template1(neg);
}

make_EHelper(adc) {
  rtl_add(&t2, &id_dest->val, &id_src->val);
  rtl_sltu(&t3, &t2, &id_dest->val);
  rtl_get_CF(&t1);
  rtl_add(&t2, &t2, &t1);
  operand_write(id_dest, &t2);

  rtl_update_ZFSF(&t2, id_dest->width);

  rtl_sltu(&t0, &t2, &id_dest->val);
  rtl_or(&t0, &t3, &t0);
  rtl_set_CF(&t0);

  rtl_xor(&t0, &id_dest->val, &id_src->val);
  rtl_not(&t0);
  rtl_xor(&t1, &id_dest->val, &t2);
  rtl_and(&t0, &t0, &t1);
  rtl_msb(&t0, &t0, id_dest->width);
  rtl_set_OF(&t0);

  print_asm_template2(adc);
}

make_EHelper(sbb) {
  rtl_sub(&t2, &id_dest->val, &id_src->val);
  rtl_sltu(&t3, &id_dest->val, &t2);
  rtl_get_CF(&t1);
  rtl_sub(&t2, &t2, &t1);
  operand_write(id_dest, &t2);

  rtl_update_ZFSF(&t2, id_dest->width);

  rtl_sltu(&t0, &id_dest->val, &t2);
  rtl_or(&t0, &t3, &t0);
  rtl_set_CF(&t0);

  rtl_xor(&t0, &id_dest->val, &id_src->val);
  rtl_xor(&t1, &id_dest->val, &t2);
  rtl_and(&t0, &t0, &t1);
  rtl_msb(&t0, &t0, id_dest->width);
  rtl_set_OF(&t0);

  print_asm_template2(sbb);
}

make_EHelper(mul) {
  rtl_lr(&t0, R_EAX, id_dest->width);
  rtl_mul(&t0, &t1, &id_dest->val, &t0);

  switch (id_dest->width) {
    case 1:
      rtl_sr_w(R_AX, &t1);
      break;
    case 2:
      rtl_sr_w(R_AX, &t1);
      rtl_shri(&t1, &t1, 16);
      rtl_sr_w(R_DX, &t1);
      break;
    case 4:
      rtl_sr_l(R_EDX, &t0);
      rtl_sr_l(R_EAX, &t1);
      break;
    default: assert(0);
  }

  print_asm_template1(mul);
}

// imul with one operand
make_EHelper(imul1) {
  rtl_lr(&t0, R_EAX, id_dest->width);
  rtl_imul(&t0, &t1, &id_dest->val, &t0);

  switch (id_dest->width) {
    case 1:
      rtl_sr_w(R_AX, &t1);
      break;
    case 2:
      rtl_sr_w(R_AX, &t1);
      rtl_shri(&t1, &t1, 16);
      rtl_sr_w(R_DX, &t1);
      break;
    case 4:
      rtl_sr_l(R_EDX, &t0);
      rtl_sr_l(R_EAX, &t1);
      break;
    default: assert(0);
  }

  print_asm_template1(imul);
}

// imul with two operands
make_EHelper(imul2) {
  rtl_sext(&id_src->val, &id_src->val, id_src->width);
  rtl_sext(&id_dest->val, &id_dest->val, id_dest->width);

  rtl_imul(&t0, &t1, &id_dest->val, &id_src->val);
  operand_write(id_dest, &t1);

  print_asm_template2(imul);
}

// imul with three operands
make_EHelper(imul3) {
  rtl_sext(&id_src->val, &id_src->val, id_src->width);
  rtl_sext(&id_src2->val, &id_src2->val, id_src->width);
  rtl_sext(&id_dest->val, &id_dest->val, id_dest->width);

  rtl_imul(&t0, &t1, &id_src2->val, &id_src->val);
  operand_write(id_dest, &t1);

  print_asm_template3(imul);
}

make_EHelper(div) {
  switch (id_dest->width) {
    case 1:
      rtl_li(&t1, 0);
      rtl_lr_w(&t0, R_AX);
      break;
    case 2:
      rtl_lr_w(&t0, R_AX);
      rtl_lr_w(&t1, R_DX);
      rtl_shli(&t1, &t1, 16);
      rtl_or(&t0, &t0, &t1);
      rtl_li(&t1, 0);
      break;
    case 4:
      rtl_lr_l(&t0, R_EAX);
      rtl_lr_l(&t1, R_EDX);
      break;
    default: assert(0);
  }

  rtl_div(&t2, &t3, &t1, &t0, &id_dest->val);

  rtl_sr(R_EAX, id_dest->width, &t2);
  if (id_dest->width == 1) {
    rtl_sr_b(R_AH, &t3);
  }
  else {
    rtl_sr(R_EDX, id_dest->width, &t3);
  }

  print_asm_template1(div);
}

make_EHelper(idiv) {
  rtl_sext(&id_dest->val, &id_dest->val, id_dest->width);

  switch (id_dest->width) {
    case 1:
      rtl_lr_w(&t0, R_AX);
      rtl_sext(&t0, &t0, 2);
      rtl_msb(&t1, &t0, 4);
      rtl_sub(&t1, &tzero, &t1);
      break;
    case 2:
      rtl_lr_w(&t0, R_AX);
      rtl_lr_w(&t1, R_DX);
      rtl_shli(&t1, &t1, 16);
      rtl_or(&t0, &t0, &t1);
      rtl_msb(&t1, &t0, 4);
      rtl_sub(&t1, &tzero, &t1);
      break;
    case 4:
      rtl_lr_l(&t0, R_EAX);
      rtl_lr_l(&t1, R_EDX);
      break;
    default: assert(0);
  }

  rtl_idiv(&t2, &t3, &t1, &t0, &id_dest->val);

  rtl_sr(R_EAX, id_dest->width, &t2);
  if (id_dest->width == 1) {
    rtl_sr_b(R_AH, &t3);
  }
  else {
    rtl_sr(R_EDX, id_dest->width, &t3);
  }

  print_asm_template1(idiv);
}
