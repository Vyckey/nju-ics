#include "cpu/exec.h"

static inline void logic_set_eflags(const rtlreg_t* result, int width) {
  rtlreg_t t;
  rtl_update_ZFSF(result, width);
  rtl_li(&t, 0);
  rtl_set_CF(&t);
  rtl_set_OF(&t);
}

static inline void shift_set_cf(const rtlreg_t* result, int n) {
  rtlreg_t t = (*result >> n) & 0x1;
  rtl_set_CF(&t);
}
make_EHelper(test) {
  rtl_and(&t0, &id_dest->val, &id_src->val);
  
  logic_set_eflags(&t0, id_dest->width);
  print_asm_template2(test);
}

make_EHelper(and) {
  rtl_and(&t0, &id_dest->val, &id_src->val);
  operand_write(id_dest, &t0);

  logic_set_eflags(&t0, id_dest->width);
  print_asm_template2(and);
}

make_EHelper(xor) {
  rtl_xor(&t0, &id_dest->val, &id_src->val);
  operand_write(id_dest, &t0);

  logic_set_eflags(&t0, id_dest->width);
  print_asm_template2(xor);
}

make_EHelper(or) {
  rtl_or(&t0, &id_dest->val, &id_src->val);
  operand_write(id_dest, &t0);

  logic_set_eflags(&t0, id_dest->width);
  print_asm_template2(or);
}

make_EHelper(sar) {
  if (id_src->val != 0) shift_set_cf(&id_dest->val, (id_src->val & 0x1f) - 1);
  rtl_sar(&t0, &id_dest->val, &id_src->val);
  operand_write(id_dest, &t0);
  // unnecessary to update CF and OF in NEMU
  rtl_update_ZFSF(&t0, id_dest->width);

  print_asm_template2(sar);
}

make_EHelper(shl) {
  if (id_src->val != 0) shift_set_cf(&id_dest->val, 32 - (id_src->val & 0x1f));
  rtl_shl(&t0, &id_dest->val, &id_src->val);
  operand_write(id_dest, &t0);
  // unnecessary to update CF and OF in NEMU
  rtl_update_ZFSF(&t0, id_dest->width);
  print_asm_template2(shl);
}

make_EHelper(shr) {
  if (id_src->val != 0) shift_set_cf(&id_dest->val, (id_src->val & 0x1f) - 1);
  rtl_shr(&t0, &id_dest->val, &id_src->val);
  operand_write(id_dest, &t0);
  // unnecessary to update CF and OF in NEMU
  rtl_update_ZFSF(&t0, id_dest->width);
  print_asm_template2(shr);
}

make_EHelper(setcc) {
  uint8_t subcode = decoding.opcode & 0xf;
  rtl_setcc(&t2, subcode);
  operand_write(id_dest, &t2);

  print_asm("set%s %s", get_cc_name(subcode), id_dest->str);
}

make_EHelper(not) {
  rtl_not(&id_dest->val);
  operand_write(id_dest, &id_dest->val);

  print_asm_template1(not);
}
