#include "apic.h"
#include "cpu.h"
#include "emu.h"
#include "paging.h"
#include "serial.h"
#include <stdint.h>

void enable_apic() {
  magic_breakpoint();
  apic_set_base(apic_get_base());

  apic_set_reg(SPURIOUS_INTERRUPT_VECTOR_OFFSET,
               apic_get_reg(SPURIOUS_INTERRUPT_VECTOR_OFFSET) | 0x100);
}

bool apic_init(void) {
  uint32_t eax, unused, edx;
  cpuid(1, &eax, &unused, &unused, &edx);
  if (!(edx & CPUID_FEAT_EDX_APIC)) {
    return false;
  }

  outb(PIC_COMMAND_MASTER, ICW_1);
  outb(PIC_COMMAND_SLAVE, ICW_1);
  outb(PIC_DATA_MASTER, ICW_2_M);
  outb(PIC_DATA_SLAVE, ICW_2_S);
  outb(PIC_DATA_MASTER, ICW_3_M);
  outb(PIC_DATA_SLAVE, ICW_3_S);
  outb(PIC_DATA_MASTER, ICW_4);
  outb(PIC_DATA_SLAVE, ICW_4);
  outb(PIC_DATA_MASTER, 0xFF);
  outb(PIC_DATA_SLAVE, 0xFF);

  enable_apic();
  return true;
}

void apic_set_base(uintptr_t apic) {
  uint32_t edx = 0;
  uint32_t eax = (apic & 0xffff0000) | IA32_APIC_BASE_MSR_ENABLE;

  edx = (apic >> 32) & 0x0f;

  cpu_set_msr(IA32_APIC_BASE_MSR, eax, edx);
}

uintptr_t apic_get_base(void) {
  uint32_t eax, edx;
  cpu_get_msr(IA32_APIC_BASE_MSR, &eax, &edx);
  return (eax & 0xfffff000) | ((uintptr_t)(edx & 0x0f) << 32);
}

uint32_t apic_get_reg(uint8_t offset) {
  uintptr_t apic = apic_get_base();
  uintptr_t reg = apic + offset + PAGING_VIRTUAL_OFFSET;
  CPU_MEMORY_BARRIER;
  return *((uint32_t *)reg);
}

void apic_set_reg(uint8_t offset, uint32_t value) {
  CPU_MEMORY_BARRIER;
  *(uint32_t *)(apic_get_base() + offset + PAGING_VIRTUAL_OFFSET) = value;
}
