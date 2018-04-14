#include "cpu/rtl.h"

/* Condition Code */

void rtl_setcc(rtlreg_t* dest, uint8_t subcode) {
  bool invert = subcode & 0x1;
  enum {
    CC_O, CC_NO, CC_B,  CC_NB,
    CC_E, CC_NE, CC_BE, CC_NBE,
    CC_S, CC_NS, CC_P,  CC_NP,
    CC_L, CC_NL, CC_LE, CC_NLE
  };

  // TODO: Query EFLAGS to determine whether the condition code is satisfied.
  // dest <- ( cc is satisfied ? 1 : 0)
  printf("subcode = 0x%04x\n", subcode);
  switch (subcode & 0xe) {
    case CC_O:
		rtl_get_OF(dest);
		// printf("t0 = 0x%04x\tdest = 0x%04x\n", t0, *dest);
		break;
    case CC_B:
		rtl_get_CF(dest);
		// printf("t0 = 0x%04x\tdest = 0x%04x\n", t0, *dest);
		break;
    case CC_E:
		rtl_get_ZF(dest);
		// printf("t0 = 0x%04x\tdest = 0x%04x\n", t0, *dest);
		break;
    case CC_BE:
		rtl_get_CF(&t0);
		rtl_get_ZF(&t1);
		rtl_or(&t0, &t0, &t1);
		// printf("t0 = 0x%04x\tdest = 0x%04x\n", t0, *dest);
		rtl_andi(&t0, &t0, 0x1);
		rtl_mv(dest, &t0);
		break;
    case CC_S:
		rtl_get_SF(dest);
		// printf("t0 = 0x%04x\tdest = 0x%04x\n", t0, *dest);
		break;
    case CC_L:
		rtl_get_SF(&t0);
		rtl_get_OF(&t1);
		rtl_xor(&t0, &t0, &t1);
		rtl_andi(&t0, &t0, 0x1);
		rtl_mv(dest, &t0);
		// printf("t0 = 0x%04x\tdest = 0x%04x\n", t0, *dest);
		break;
    case CC_LE:
		rtl_get_SF(&t1);
		rtl_get_OF(&t2);
		rtl_get_ZF(&t3);
		rtl_xor(&t0, &t1, &t2);
		rtl_or(&t0, &t0, &t3);
		rtl_andi(&t0, &t0, 0x1);
		rtl_mv(dest, &t0);
		// printf("t0 = 0x%04x\tdest = 0x%04x\n", t0, *dest);
		break;
      // TODO();
    default: panic("should not reach here");
    case CC_P: panic("n86 does not have PF");
  }

  if (invert) {
    rtl_xori(dest, dest, 0x1);
  }
}
