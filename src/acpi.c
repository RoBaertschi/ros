#include "acpi.h"
#include "paging.h"
#include "serial.h"
#include <stdbool.h>
#include <stddef.h>
#include <stdint.h>

bool validate_rsdp(uint8_t *byte_array, size_t size) {
  uint32_t sum = 0;
  for (size_t i = 0; i < size; i++) {
    sum += byte_array[i];
  }
  return (sum & 0xFF) == 0;
}


acpi_sdt_header* acpi_get_header(struct limine_rsdp_response* response) {

  if (((rsdp_descriptor*)response->address)->revision == 0) {
    rsdp_descriptor* desc = response->address;

    const char* expected_signature = "RSD PTR ";
    for (int i = 0; i < 8; i++) {
      if (expected_signature[i] != desc->signature[i]) {
        kprint_string("rsdp signature invalid");
        return NULL;
      }
    }

    uintptr_t rsdt_address = ((uintptr_t)desc->rsdt_address);
    rsdt* rsdt = (void*)rsdt_address + PAGING_VIRTUAL_OFFSET;
    acpi_sdt_header ash = rsdt->sdt_header;
    expected_signature = "RSDT";
    for (size_t i = 0; i < 4; i++) {
      if (expected_signature[i] != ash.signature[i]) {
        kprint_string("acpi sdt header is invalid or not a rsdp");
        return NULL;
      }
    }
    return &rsdt->sdt_header;
  } else {

    rsdp2_descriptor* desc = response->address;

    const char* expected_signature = "RSD PTR ";
    for (int i = 0; i < 8; i++) {
      if (expected_signature[i] != desc->signature[i]) {
        kprint_string("rsdp signature invalid");
        return NULL;
      }
    }

    xsdt* xsdt = (struct xsdt*)((uintptr_t)desc->xsdt_address + PAGING_VIRTUAL_OFFSET);

    acpi_sdt_header* ash = &xsdt->sdt_header;
    expected_signature = "XSDT";
    for (size_t i = 0; i < 4; i++) {
      if (expected_signature[i] != ash->signature[i]) {
        kprint_string("acpi sdt header is invalid or not a xsdt");
        return NULL;
      }
    }
    return ash;
  }
}
