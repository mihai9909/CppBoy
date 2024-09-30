#pragma once

#include <QWidget>
#include <QPainter>
#include "memory.h"
#include <vector>

#define PIXEL_SIZE 4 // width and height of one gameboy pixel
#define VRAM_START 0x8000 // start address of VRAM
#define VRAM_LEN 0x1800 // length of VRAM
#define VRAM_END (VRAM_START + VRAM_LEN)
#define BTILE_SIZE 0x10 // size of one tile in bytes

// Picture Processing Unit responsible for rendering graphics inside a window
class PPU : public QWidget
{
    Q_OBJECT

public:
    PPU(Memory* memory, QWidget* parent = nullptr);
    ~PPU();

protected:
    void paintEvent(QPaintEvent* event);
    void drawTile(std::vector<WORD> tile, QPoint tileOffset, std::vector<QBrush> palette);
    std::vector<WORD> buildTile(std::vector<BYTE> bytes);
    void displayTileMap();

    Memory* memory;
};
