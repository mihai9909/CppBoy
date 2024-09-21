#include "memory.h"

Memory::Memory() 
{
}

Memory::~Memory() {}

void Memory::loadMemory(QString filePath) 
{
	QByteArray byteArr = FileUtil::readBinaryFile(filePath);
	bank1 = std::vector<BYTE>(byteArr.begin(), byteArr.end());
}
