#ifndef ROS_EMU_H
#define ROS_EMU_H

static inline void magic_breakpoint(void) {
  asm volatile("xchgw %bx, %bx");
}

#endif // ROS_EMU_H
