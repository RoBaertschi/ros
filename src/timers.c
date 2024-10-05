#include "timers.h"
#include "serial.h"


void pit_set_periodic(uint16_t count) {
  outb(PIT_COMMAND_PORT, 0b00110100);
  outb(PIT_CHANNEL_PORT, count & 0xFF);
  outb(PIT_CHANNEL_PORT, count >> 8);
}
