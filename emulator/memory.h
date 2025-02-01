#pragma once

#include "../types.h"
#include "../utils/file_util.h"
#include "../emulator/cartridge.h"
#include "memory_map.h"
#include <vector>
#include <QString>
#include <QByteArray>

#define MEM_SIZE 65536 // in bytes

class Memory {
public:
	Memory();
	~Memory();
	void loadCartridge(Cartridge* cartridge);
	std::vector<BYTE> readByteSequence(int offset, int length);
	BYTE readByte(WORD address);
	void flush();
	BYTE* readPByte(WORD address);
	void setByte(WORD address, BYTE value);

private:
	std::vector<BYTE> bank1;
};
