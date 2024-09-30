#pragma once

#include <QWidget>
#include <QPainter>
#include "memory.h"
#include <vector>

#define PIXEL_SIZE 5 // width and height of one gameboy pixel

// Picture Processing Unit responsible for rendering graphics inside a window
class PPU : public QWidget
{
    Q_OBJECT

public:
    PPU(Memory* memory, QWidget* parent = nullptr);
    ~PPU();

protected:
    void paintEvent(QPaintEvent* event);
    void drawTile(std::vector<WORD> tile);
    std::vector<WORD> buildTile(std::vector<BYTE> bytes);

    Memory* memory;
};
