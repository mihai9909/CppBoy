#include "header_parser.h"

HeaderParser::HeaderParser() {}

std::string HeaderParser::getTitle()
{
    std::string title(&header[TITLE_START_ADDR], &header[TITLE_END_ADDR]);

    //resize to exclude tracing 0x00
    size_t nullPos = title.find('\0');
    if (nullPos != std::string::npos) {
        title.resize(nullPos);
    }

    return title;
}

std::string HeaderParser::getManufacturer()
{
	return std::string();
}

void HeaderParser::loadHeader(std::string ROMContent) {
    header = std::vector<BYTE>(ROMContent.begin(), ROMContent.end());
}

// returns size of ROM in KBs
int HeaderParser::getSize()
{
	return 32 * 1 << header[ROM_SIZE_ADDR];
}
