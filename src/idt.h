#ifndef ROS_IDT_H_
#define ROS_IDT_H_

#include <stdint.h>
#include <stddef.h>
struct interrupt_descriptor {
    uint16_t address_low;
    uint16_t selector;
    uint8_t ist;
    // 3:0 type -> trap: 0b1111 or interrupt: 0b1110
    // 4 -> 0
    // 6:5 DPL -> ring that can trigger the inerrupt
    // 7 Present -> 1 means this descriptor is valid
    uint8_t flags;
    uint16_t address_mid;
    uint32_t address_high;
    uint32_t reserved;
} __attribute__((packed));
typedef struct interrupt_descriptor interrupt_descriptor;

struct idtr {
    uint16_t limit;
    uint64_t base;
} __attribute__((packed));
typedef struct idtr idtr;

struct interrupt_dispatch_context {

    uint64_t r15;
    uint64_t r14;
    uint64_t r13;
    uint64_t r12;
    uint64_t r11;
    uint64_t r10;
    uint64_t r9;
    uint64_t r8;
    uint64_t rsi;
    uint64_t rdx;
    uint64_t rdi;
    uint64_t rcx;
    uint64_t rbx;
    uint64_t rax;

    uint64_t vector_number;
    uint64_t error_code;

    uint64_t iret_rip;
    uint64_t iret_cs;
    uint64_t iret_flags;
    uint64_t iret_rsp;
    uint64_t iret_ss;
};
typedef struct interrupt_dispatch_context interrupt_dispatch_context;

#define IDT_LENGTH 256

void set_idt_entry(uint8_t vector, uint64_t handler, uint8_t dpl);
void load_idt(void);
void init_idt(void);
void dump_idt(void);

#endif // ROS_IDT_H_
