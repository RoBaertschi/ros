#ifndef ROS_SERIAL_H_
#define ROS_SERIAL_H_

#include <stdint.h>
#define PORT 0x3f8 // COM1


int init_serial();

unsigned char inb(int portnum);
void outb(int portnum, unsigned char data);

inline void kprint_char(unsigned char ch) {
    outb(PORT, ch);
}
void kprint_string(char *string);
void kprint_uint(unsigned int number);
void kprint_int(int number);
void kprint_octal_uint(unsigned int number);
void kprint_hex_uint(unsigned int number);
void kprint_hex_uint64(uint64_t number);


#endif // ROS_SERIAL_H_
