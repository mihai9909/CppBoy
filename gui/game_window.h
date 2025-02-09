#pragma once

#include <QWidget>
#include <QPainter>
#include <vector>
#include "../emulator/memory_map.h"
#include "../types.h"

#define PIXEL_SIZE 2 // width and height of one gameboy pixel
#define TILES_PER_ROW 16

class GameWindow : public QWidget
{
	Q_OBJECT

public:
	GameWindow(QWidget* parent = nullptr);
	void paintEvent(QPaintEvent* event);
	void drawTile(std::vector<WORD> tile, QPoint tileOffset, std::vector<QBrush> palette);
	void setFrameBuffer(std::vector<std::vector<WORD>> frameBuffer);

protected:
	std::vector<std::vector<WORD>> frameBuffer;
};