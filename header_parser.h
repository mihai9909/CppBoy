#pragma once
#include <unordered_map>
#include <vector>
#include "types.h"

// Addresses of cartridge metadata inside the header
#define HEADER_START_ADDR 0x0100
#define HEADER_END_ADDR 0x014F
#define ROM_SIZE_ADDR 0x0148
#define TITLE_START_ADDR 0x0134
#define TITLE_END_ADDR 0x0143
#define PUBLISHER_ADDR 0x014B

class HeaderParser {
public:
	HeaderParser();
	void loadHeader(std::string ROMcontent);
	std::string getTitle();
	int getSize();
	std::string getPublisher();

private:
	std::vector<BYTE> header;
	std::unordered_map<int, std::string> publishers;
};
