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

class HeaderParser {
public:
	HeaderParser();
	void loadHeader(std::string ROMcontent);
	std::string getTitle();
	std::string getManufacturer();
	int getSize();

private:
	std::vector<BYTE> header;
};
