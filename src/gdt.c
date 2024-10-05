#include "gdt.h"
#include <stdint.h>

// We use 5 selectors
uint64_t gdt_entries[5] = {0};
uint64_t gdt_size = sizeof(gdt_entries) / sizeof(uint64_t);

struct GDTR {
  uint16_t limit;
  uint64_t address;
} __attribute__((packed));
typedef struct GDTR gdtr;

void reloadSegments(void);

void init_gdt(void) {
  gdt_entries[0] = 0;

  uint64_t kernel_code = 0;
  kernel_code |= 0b1011 << 8;
  kernel_code |= 1 << 12;
  kernel_code |= 0 << 13;
  kernel_code |= 1 << 15;
  kernel_code |= 1 << 21;
  gdt_entries[1] = kernel_code << 32;

  uint64_t kernel_data = 0;
  kernel_data |= 0b0011 << 8;
  kernel_data |= 1 << 12;
  kernel_data |= 0 << 13;
  kernel_data |= 1 << 15;
  kernel_data |= 1 << 21;
  gdt_entries[2] = kernel_data << 32;

  uint64_t user_code = kernel_code | (3 << 13);
  gdt_entries[3] = user_code << 32;

  uint64_t user_data = kernel_data | (3 << 13);
  gdt_entries[4] = user_data << 32;

  gdtr gdtr = {
      .limit = gdt_size * sizeof(uint64_t) - 1,
      .address = (uint64_t)gdt_entries,
  };

  asm("lgdt %0" : : "m"(gdtr));
  reloadSegments();
}
