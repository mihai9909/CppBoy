#pragma once

#include "../types.h"
#include "../utils/file_util.h"
#include "../emulator/cartridge.h"
#include <vector>
#include <QString>
#include <QByteArray>

#define MEM_SIZE 65536 // in bytes
#define LCDC 0xFF40 // LCD control register

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
