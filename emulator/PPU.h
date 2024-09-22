#pragma once

#include <QWidget>
#include <QPainter>

// Picture Processing Unit responsible for rendering graphics inside a window
class PPU : public QWidget
{
    Q_OBJECT

public:
    PPU(QWidget* parent = nullptr);
    ~PPU();

protected:
    void paintEvent(QPaintEvent* event);
};
