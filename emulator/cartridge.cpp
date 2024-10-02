#include "cartridge.h"

Cartridge::Cartridge(QString filePath)
{
	QByteArray byteArr = FileUtil::readBinaryFile(filePath);

	this->rom = std::vector<BYTE>(byteArr.begin(), byteArr.end());
}

Cartridge::~Cartridge() {}

std::vector<BYTE> Cartridge::getContents() {
	return rom;
}