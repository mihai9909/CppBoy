#include "memory.h"

Memory::Memory()
{
	bank1 = std::vector<BYTE>(MEM_SIZE, 0);
}

Memory::~Memory() {}

void Memory::loadMemory(QString filePath)
{
	QByteArray byteArr = FileUtil::readBinaryFile(filePath);

	// Make sure not to copy more than MEM_SIZE
	std::size_t copySize = std::min(static_cast<std::size_t>(byteArr.size()), static_cast<std::size_t>(MEM_SIZE));

	std::copy(byteArr.begin(), byteArr.begin() + copySize, bank1.begin());
}

std::vector<BYTE> Memory::getByteSequence(int offset, int length) {
	return std::vector<BYTE>(bank1.begin() + offset, bank1.begin() + offset + length);
}
