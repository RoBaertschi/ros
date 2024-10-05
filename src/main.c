#include "acpi.h"
#include "apic.h"
#include "gdt.h"
#include "idt.h"
#include "interrupts.h"
#include "limine.h"
#include "serial.h"
#include "timers.h"
#include <stdbool.h>
#include <stddef.h>
#include <stdint.h>

// Set the base revision to 2, this is recommended as this is the latest
// base revision described by the Limine boot protocol specification.
// See specification for further info.

__attribute__((used,
               section(".requests"))) static volatile LIMINE_BASE_REVISION(2);

// The Limine requests can be placed anywhere, but it is important that
// the compiler does not optimise them away, so, usually, they should
// be made volatile or equivalent, _and_ they should be accessed at least
// once or marked as used with the "used" attribute as done here.

__attribute__((
    used,
    section(".requests"))) static volatile struct limine_framebuffer_request
    framebuffer_request = {.id = LIMINE_FRAMEBUFFER_REQUEST, .revision = 0};

__attribute__((used,
               section(".requests"))) static volatile struct limine_rsdp_request
    rsdp_request = {.id = LIMINE_RSDP_REQUEST, .revision = 0};

// Finally, define the start and end markers for the Limine requests.
// These can also be moved anywhere, to any .c file, as seen fit.

__attribute__((used,
               section(".requests_start_"
                       "marker"))) static volatile LIMINE_REQUESTS_START_MARKER;

__attribute__((
    used,
    section(
        ".requests_end_marker"))) static volatile LIMINE_REQUESTS_END_MARKER;

// GCC and Clang reserve the right to generate calls to the following
// 4 functions even if they are not directly called.
// Implement them as the C specification mandates.
// DO NOT remove or rename these functions, or stuff will eventually break!
// They CAN be moved to a different .c file.

void *memcpy(void *dest, const void *src, size_t n) {
  uint8_t *pdest = (uint8_t *)dest;
  const uint8_t *psrc = (const uint8_t *)src;

  for (size_t i = 0; i < n; i++) {
    pdest[i] = psrc[i];
  }

  return dest;
}

void *memset(void *s, int c, size_t n) {
  uint8_t *p = (uint8_t *)s;

  for (size_t i = 0; i < n; i++) {
    p[i] = (uint8_t)c;
  }

  return s;
}

void *memmove(void *dest, const void *src, size_t n) {
  uint8_t *pdest = (uint8_t *)dest;
  const uint8_t *psrc = (const uint8_t *)src;

  if (src > dest) {
    for (size_t i = 0; i < n; i++) {
      pdest[i] = psrc[i];
    }
  } else if (src < dest) {
    for (size_t i = n; i > 0; i--) {
      pdest[i - 1] = psrc[i - 1];
    }
  }

  return dest;
}

int memcmp(const void *s1, const void *s2, size_t n) {
  const uint8_t *p1 = (const uint8_t *)s1;
  const uint8_t *p2 = (const uint8_t *)s2;

  for (size_t i = 0; i < n; i++) {
    if (p1[i] != p2[i]) {
      return p1[i] < p2[i] ? -1 : 1;
    }
  }

  return 0;
}

// Halt and catch fire function.
static void hcf(void) {
  for (;;) {
    asm("hlt");
  }
}

// The following will be our kernel's entry point.
// If renaming kmain() to something else, make sure to change the
// linker script accordingly.
void kmain(void) {
  if (init_serial() != 0) {
    hcf();
  }

  kprint_string("Hello World\n");

  // Ensure the bootloader actually understands our base revision (see spec).
  if (LIMINE_BASE_REVISION_SUPPORTED == false) {
    kprint_string("limine version is not supported\n");
    hcf();
  }

  // Ensure we got a framebuffer.
  if (framebuffer_request.response == NULL ||
      framebuffer_request.response->framebuffer_count < 1) {
    kprint_string(
        "no response for framebuffer request or not enough frame buffers\n");
    hcf();
  }

  kprint_string("initializing gdt\n");
  disable_interrupts();
  init_gdt();
  kprint_string("initialized gdt\n");
  kprint_string("initializing idt\n");
  init_idt();
  kprint_string("initialized idt\n");
  enable_interrupts();

  if (rsdp_request.response == NULL || rsdp_request.response->address == NULL) {
    kprint_string("rsdp is not available.\n");
    hcf();
  }
  if (
      !validate_rsdp(rsdp_request.response->address,
                    ((rsdp_descriptor*)(rsdp_request.response->address))->revision == 0
                        ? sizeof(rsdp_descriptor)
                        : sizeof(rsdp2_descriptor))) {
    kprint_string("rsdp is invalid.\n");
    hcf();
  }

  acpi_sdt_header* ash = acpi_get_header(rsdp_request.response);
  if (ash == NULL) {
    hcf();
  }

  if(!apic_init()) {
    kprint_string("could not initalize apic");
    hcf();
  }

  pit_set_periodic(1193);

  // Fetch the first framebuffer.
  struct limine_framebuffer *framebuffer =
      framebuffer_request.response->framebuffers[0];

  // Note: we assume the framebuffer model is RGB with 32-bit pixels.
  for (size_t i = 0; i < 100; i++) {
    volatile uint32_t *fb_ptr = framebuffer->address;
    fb_ptr[i * (framebuffer->pitch / 4) + i] = 0xffffff;
  }

  kprint_uint(1234);
  kprint_string("\n");
  kprint_int(-120);
  kprint_string("\n");
  kprint_octal_uint(0120);
  kprint_string("\n");
  kprint_hex_uint(0xAA);
  kprint_string("\n");
  // We're done, just hang...
  kprint_string("exiting\n");

  while (true);
}
