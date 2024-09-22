#include "PPU.h"

PPU::PPU(QWidget* parent)
{
	QPalette pal = QPalette();

	pal.setColor(QPalette::Window, Qt::black);

	setAutoFillBackground(true);
	setPalette(pal);
}

PPU::~PPU() {}

void PPU::paintEvent(QPaintEvent* event)
{
	Q_UNUSED(event);

	QPainter painter;
	painter.begin(this);

	painter.setPen(QColor(255, 0, 0));
	painter.setBrush(QColor(255, 0, 0));

	painter.fillRect(QRect(10, 10, 30, 30), painter.brush());
	painter.end();
}