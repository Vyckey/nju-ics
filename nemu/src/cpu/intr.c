#include "cpu/exec.h"
#include "memory/mmu.h"

void raise_intr(uint8_t NO, vaddr_t ret_addr) {
  /* TODO: Trigger an interrupt/exception with ``NO''.
   * That is, use ``NO'' to index the IDT.
   */
	rtl_push(&cpu.eflags_val);
	rtl_push(&ret_addr);
	vaddr_t gate_addr = cpu.idtr + sizeof(GateDesc)*NO;
	uint32_t low_addr = vaddr_read(gate_addr, 2) & 0xffff,
		high_addr = vaddr_read(gate_addr + 4, 4) & 0xffff0000;
	decoding.jmp_eip = high_addr | low_addr;
	decoding.is_jmp = 1;
}

void dev_raise_intr() {
}
