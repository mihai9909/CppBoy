#pragma once

#include "../types.h"
#include "../utils/file_util.h"
#include <vector>
#include <QString>
#include <QByteArray>

#define MEM_SIZE 65536 // in bytes
#define LCDC 0xFF40 // LCD control register

class Memory {
public:
	Memory();
	~Memory();
	void loadMemory(QString filePath);
	std::vector<BYTE> getByteSequence(int offset, int length);

private:
	std::vector<BYTE> bank1;
};
