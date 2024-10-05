#ifndef ROS_TIMERS_H_
#define ROS_TIMERS_H_

#include <stdint.h>

#define PIT_COMMAND_PORT 0x43
#define PIT_CHANNEL_PORT 0x40

void pit_set_periodic(uint16_t count);

#endif // ROS_TIMERS_H_
