#ifndef ROS_ACPI_H_
#define ROS_ACPI_H_

#include "limine.h"
#include <stdint.h>
#include <stdbool.h>
#include <stddef.h>

struct rsdp_descriptor {
  // string that should be "RSD PTR "
  uint8_t signature[8];
  uint8_t checksum;
  // OEM
  uint8_t OEMID[6];
  uint8_t revision;
  uint32_t rsdt_address;
} __attribute__((packed));
typedef struct rsdp_descriptor rsdp_descriptor;

struct rsdp2_descriptor {
  uint8_t signature[8];
  uint8_t checksum;
  uint8_t OEMID[6];
  uint8_t revision;
  uint32_t rsdt_address;

  // v2
  uint32_t length;
  uint64_t xsdt_address;
  uint8_t extended_checksum;
  uint8_t reserved [3];
} __attribute__((packed));
typedef struct rsdp2_descriptor rsdp2_descriptor;

struct acpi_sdt_header {
  uint8_t signature[4];
  uint32_t length;
  uint8_t revision;
  uint8_t checksum;
  uint8_t OEMID[6];
  uint8_t OEM_table_ID[8];
  uint32_t OEM_revision;
  uint32_t creator_id;
  uint32_t creator_revision;
} __attribute__((packed));
typedef struct acpi_sdt_header acpi_sdt_header;

struct rsdt {
  // Signature should be "RSDP"
  acpi_sdt_header sdt_header;
  uint32_t sdt_addresses[];
}__attribute__((packed));
typedef struct rsdt rsdt;

struct xsdt {
  // Signature should be "XSDT"
  acpi_sdt_header sdt_header;
  uint64_t sdt_addresses[];
}__attribute__((packed));
typedef struct xsdt xsdt;

bool validate_rsdp(uint8_t *byte_array, size_t size);
acpi_sdt_header* acpi_get_header(struct limine_rsdp_response* response);

#endif // ROS_ACPI_H_
