#include "types.h"
#include <iostream>

void print_header_packet(tinfoil_usb_header_packet& header_packet)
{
	std::cout << "Tinfoil Header Information\n";
	std::cout << "Magic: " << header_packet.magic << std::endl;
	std::cout << "Command: " << header_packet.command << std::endl;
	std::cout << "Size: " << header_packet.size << std::endl;
	std::cout << "Packet Count: " << header_packet.packet_count << std::endl;
	std::cout << "Packet Index: " << header_packet.packet_idx << std::endl;
	std::cout << "Thread ID: " << header_packet.thread_id << std::endl;
	std::cout << "Timestamp: " << header_packet.timestamp << std::endl;
}
