#pragma once

#include "memory.h"
#include <vector>

#define BTILE_SIZE 0x10 // size of one tile in bytes

// Picture Processing Unit responsible for rendering graphics inside a window
class PPU
{
    typedef std::vector<WORD> Tile;

public:
    PPU(Memory* memory);
    ~PPU();
    void updateFrameBuffer();
    void setRenderCallback(std::function<void(const std::vector<std::vector<WORD>>&)> callback);

protected:
    std::function<void(const std::vector<std::vector<WORD>>&)> renderCallback;
    std::vector<WORD> buildTile(std::vector<BYTE> bytes);
    std::vector<BYTE> readTile(BYTE tileIndex);
    void displayTileMap();

    Memory* memory;
    std::vector<Tile> backgroundMap;
    std::vector<Tile> frameBuffer;
};
