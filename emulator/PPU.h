#pragma once

#include <QWidget>
#include <QPainter>
#include "memory.h"

// Picture Processing Unit responsible for rendering graphics inside a window
class PPU : public QWidget
{
    Q_OBJECT

public:
    PPU(Memory* memory, QWidget* parent = nullptr);
    ~PPU();

protected:
    void paintEvent(QPaintEvent* event);

    Memory* memory;
};
