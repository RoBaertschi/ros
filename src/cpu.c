#include "cpu.h"
#include <cpuid.h>


int cpuid(uint32_t leaf, uint32_t* eax, uint32_t* ebx, uint32_t* ecx, uint32_t* edx) {
  return __get_cpuid(leaf, eax, ebx, ecx, edx);
}

bool cpu_has_msr(void) {
  uint32_t eax, edx, unused;
  cpuid(1, &eax, &unused, &unused, &edx);
  return edx & CPUID_FEAT_EDX_MSR;
}

void cpu_get_msr(uint32_t msr, uint32_t *lo, uint32_t *hi) {
  asm volatile("rdmsr" : "=a"(*lo), "=d"(*hi) : "c"(msr));
}

void cpu_set_msr(uint32_t msr, uint32_t lo, uint32_t hi) {
  asm volatile("wrmsr" : : "a"(lo), "d"(hi), "c"(msr));
}
