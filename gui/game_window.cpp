#include "game_window.h"

GameWindow::GameWindow(QWidget* parent) {
	QPalette pal = QPalette();
	frameBuffer = std::vector<std::vector<WORD>>(160);


	pal.setColor(QPalette::Window, Qt::black);

	setAutoFillBackground(true);
	setPalette(pal);
}

void GameWindow::paintEvent(QPaintEvent* event)
{
	Q_UNUSED(event);

	for (int i = 0; i < frameBuffer.size(); i++) {
		std::vector<WORD> tile = frameBuffer[i];
		QPoint offset = QPoint(i % TILES_PER_ROW, i / TILES_PER_ROW) * 9;
		std::vector<QBrush> palette = { QBrush(QColor(8,24,32)), QBrush(QColor(52,104,86)), QBrush(QColor(136,192,112)), QBrush(QColor(224,248,208)) };
		drawTile(tile, offset, palette);
	}
}

void GameWindow::setFrameBuffer(std::vector<std::vector<WORD>> frameBuffer) {
	this->frameBuffer = frameBuffer;
	update();
}

void GameWindow::drawTile(std::vector<WORD> tile, QPoint tileOffset, std::vector<QBrush> palette) {
	QPainter painter;
	painter.begin(this);

	for (int i = 0; i < tile.size(); i++) {
		for (int j = 0; j < 8; j++) {
			int brushIndex = (tile[i] >> (2 * j)) & 0x3;
			QBrush brush = palette[brushIndex];
			QPoint pixelOffset = (QPoint((7 - j), i) + tileOffset) * PIXEL_SIZE;
			QPoint pixelSize = (QPoint((8 - j), i + 1) + tileOffset) * PIXEL_SIZE;

			painter.fillRect(QRect(pixelOffset, pixelSize), brush);
		}
	}

	painter.end();
}
