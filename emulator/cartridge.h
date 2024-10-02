#pragma once

#include "../types.h"
#include "../utils/file_util.h"
#include <vector>
#include <QString>
#include <QByteArray>

class Cartridge {
public:
	Cartridge(QString filePath);
	~Cartridge();
	std::vector<BYTE> getContents();

private:
	std::vector<BYTE> rom;
};
