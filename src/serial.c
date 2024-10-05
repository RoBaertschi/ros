#include "serial.h"
#include <stddef.h>
#include <stdint.h>

int init_serial() {
  outb(PORT + 1, 0x00);
  outb(PORT + 3, 0x80); // Enable DLAB (set baud rate divisor)
  outb(PORT + 0, 0x03); // Set divisor to 3 (lo byte) 38400 baud
  outb(PORT + 1, 0x00); //                  (hi byte)
  outb(PORT + 3, 0x03); // 8 bits, no parity, one stop bit
  outb(PORT + 2, 0xC7); // Enable FIFO, clear them, with 14-byte threshold
  outb(PORT + 4, 0x0B); // IRQs enabled, RTS/DSR set
  outb(PORT + 4, 0x1E); // Set in loopback mode, test the serial chip
  outb(PORT + 0, 0xAE); // Send a test byte

  // Check that we received the same test byte we sent
  if (inb(PORT + 0) != 0xAE) {
    return 1;
  }

  // If serial is not faulty set it in normal operation mode:
  // not-loopback with IRQs enabled and OUT#1 and OUT#2 bits enabled
  outb(PORT + 4, 0x0F);
  return 0;
}

unsigned char inb(int portnum) {
  unsigned char data = 0;
  asm volatile("inb %%dx, %%al" : "=a"(data) : "d"(portnum));

  return data;
}

void outb(int portnum, unsigned char data) {
  asm volatile("outb %%al, %%dx" ::"a"(data), "d"(portnum));
}

size_t strlen(const char *str) {
  const char* s = 0;
  for (s = str; *s; ++s);
  return (s - str);
}

void reverse(char s[]) {
  int i,j;
  char c;
  for (i = 0, j = strlen(s)-1; i<j; i++, j--) {
    c = s[i];
    s[i] = s[j];
    s[j] = c;
  }
}

void kprint_string(char *string) {
  for (int i = 0; *(string + i) != 0; i++) {
    outb(PORT, *(string + i));
  }
}

void kprint_uint(unsigned int number) {
  char s[25] = {0};

  int i = 0;
  do {
    s[i++] = number % 10 + '0';
  } while((number /= 10) > 0);
  reverse(s);

  size_t size = strlen(s);
  if (size > 25) {
    // Don't go out of bounds
    size = 25;
  }
  for (size_t i = 0; i < size; i++) {
    outb(PORT, s[i]);
  }
}

void kprint_int(int number) {
  if (number < 0) {
    outb(PORT, '-');
    kprint_uint(-number);
  } else {
    kprint_uint(number);
  }
}


void kprint_octal_uint(unsigned int number) {

  outb(PORT, '0');
  outb(PORT, 'o');

  char s[25] = {0};

  int i = 0;
  do {
    s[i++] = number % 8 + '0';
  } while((number /= 8) > 0);
  reverse(s);

  size_t size = strlen(s);
  if (size > 25) {
    // Don't go out of bounds
    size = 25;
  }
  for (size_t i = 0; i < size; i++) {
    outb(PORT, s[i]);
  }

}

void kprint_hex_uint(unsigned int number) {
  outb(PORT, '0');
  outb(PORT, 'x');

  char s[25] = {0};

  int i = 0;
  do {
    int mod = number % 16;

    if (mod <= 9) {
      s[i++] = '0' + mod;
    } else {
      s[i++] = 'A' + (mod - 10);
    }
  } while((number /= 16) > 0);
  reverse(s);
  size_t size = strlen(s);
  if (size > 25) {
    // Don't go out of bounds
    size = 25;
  }
  for (size_t i = 0; i < size; i++) {
    outb(PORT, s[i]);
  }

}

void kprint_hex_uint64(uint64_t number) {

  outb(PORT, '0');
  outb(PORT, 'x');
  char s[25] = {0};

  int i = 0;
  do {
    int mod = number % 16;

    if (mod <= 9) {
      s[i++] = '0' + mod;
    } else {
      s[i++] = 'A' + (mod - 10);
    }
  } while((number /= 16) > 0);
  reverse(s);
  size_t size = strlen(s);
  if (size > 25) {
    // Don't go out of bounds
    size = 25;
  }
  for (size_t i = 0; i < size; i++) {
    outb(PORT, s[i]);
  }
}
