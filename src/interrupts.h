#ifndef ROS_INTERRUPTS_H_
#define ROS_INTERRUPTS_H_

static inline void disable_interrupts(void) { asm volatile("cli"); }
static inline void enable_interrupts(void) { asm volatile("sti"); };

#endif // ROS_INTERRUPTS_H_
