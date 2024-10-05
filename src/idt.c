#include "idt.h"
#include "emu.h"
#include "serial.h"
#include <stddef.h>
#include <stdint.h>
__attribute__((aligned(0x10)))
interrupt_descriptor __idt[IDT_LENGTH] = {0};

extern char vector_handler_0[];

void set_idt_entry(uint8_t vector, uint64_t handler, uint8_t dpl) {
  interrupt_descriptor *entry = &__idt[vector];
  entry->address_low = handler & 0xFFFF;
  entry->address_mid = (handler >> 16) & 0xFFFF;
  entry->address_high = handler >> 32;
  entry->selector = 0x8;
  entry->flags = 0b1110 | ((dpl & 0b11) << 5) | (1 << 7);
  entry->ist = 0;

  // kprint_string("IDT Entry ");
  // kprint_int(vector);
  // kprint_string(", address_low = ");
  // kprint_hex_uint(entry->address_low);
  // kprint_string(", address_mid = ");
  // kprint_hex_uint(entry->address_mid);
  // kprint_string(", address_high = ");
  // kprint_hex_uint(entry->address_high);
  // kprint_string(".\n");
}

void load_idt(void) {
  idtr idt_reg;
  idt_reg.limit = (uint16_t)sizeof(interrupt_descriptor) * IDT_LENGTH - 1;
  idt_reg.base = (uint64_t)&__idt[0];
  asm volatile("lidt %0" ::"m"(idt_reg));
}

void init_idt(void) {
  for (size_t i = 0; i < IDT_LENGTH; i++) {
    set_idt_entry(i, (uint64_t)(vector_handler_0 + (i * 16)), 0);
  }
  load_idt();
  // dump_idt();
}

interrupt_dispatch_context *interrupt_dispatch(interrupt_dispatch_context *ctx) {
  switch (ctx->vector_number) {
  case 14:
    kprint_string("page fault ");
    kprint_int(ctx->error_code);
    for(;;){
      asm volatile("hlt");
    }
  case 8:
    kprint_string("double fault\n");
    break;
  default:
    kprint_string("unexpected interrupt\n");
    kprint_int(ctx->vector_number);
    kprint_string(".\n");
  }

  return ctx;
}
void dump_idt(void) {
  for (size_t i = 0; i < IDT_LENGTH; i++) {
    kprint_string("IDT Vector ");
    kprint_uint(i);
    kprint_string(", addr: ");
    kprint_hex_uint64(((uint64_t) __idt) + i);
    kprint_string("\n");
  }
}
