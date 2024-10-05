#ifndef ROS_APIC_H_
#define ROS_APIC_H_

#include <stdint.h>
#define PIC_COMMAND_MASTER 0x20
#define PIC_COMMAND_SLAVE 0xA0

#define PIC_DATA_MASTER 0x21
#define PIC_DATA_SLAVE 0xA1

#define ICW_1   0x11
#define ICW_2_M 0x20
#define ICW_2_S 0x28
#define ICW_3_M 0x2
#define ICW_3_S 0x4
#define ICW_4   0x01

#define IA32_APIC_BASE_MSR 0x1B
#define IA32_APIC_BASE_MSR_BSP 0x100 // Processor is a BSP
#define IA32_APIC_BASE_MSR_ENABLE 0x800
#define SPURIOUS_INTERRUPT_VECTOR_OFFSET 0xF0

#include <stdbool.h>
bool apic_init(void);
void apic_set_base(uintptr_t apic);
uintptr_t apic_get_base(void);
uint32_t apic_get_reg(uint8_t offset);
void apic_set_reg(uint8_t offset, uint32_t value);

#endif // ROS_APIC_H_
