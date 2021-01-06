#pragma once
#include <stdint.h>

constexpr uint32_t TINFOIL_USB_HEADER_MAGIC = 0x12121212;

#pragma pack (push, 1)
struct tinfoil_usb_header_packet {
	uint32_t magic;
	uint32_t command;
	uint64_t size;
	uint32_t thread_id;
	uint16_t packet_idx;
	uint16_t packet_count;
	uint64_t timestamp;
};

void print_header_packet(tinfoil_usb_header_packet& header_packet);
