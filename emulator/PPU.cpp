#include "PPU.h"

PPU::PPU(Memory* memory, QWidget* parent) : memory(memory)
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

	for (int i = VRAM_START; i < 0x9900; i += 0x10) {
		std::vector<BYTE> tileBytes = memory->getByteSequence(i, 0x10);
		std::vector<WORD> tile = buildTile(tileBytes);
		drawTile(tile, QPoint(((i - VRAM_START) / 0x20) % 18, ((i - VRAM_START) / 0x20) / 18));
	}
}

void PPU::drawTile(std::vector<WORD> tile, QPoint tileOffset) {
	QPainter painter;
	painter.begin(this);

	std::vector<QBrush> palette = { QBrush(QColor(8,24,32)), QBrush(QColor(52,104,86)), QBrush(QColor(136,192,112)), QBrush(QColor(224,248,208)) };
	tileOffset *= 8;

	for (int i = 0; i < tile.size(); i++) {
		for (int j = 0; j < 8; j++) {
			int brushIndex = (tile[i] >> (2 * j)) & 0x3;
			QBrush brush = palette[brushIndex];
			QPoint pixelOffset = (QPoint((7 - j), i) + tileOffset) * PIXEL_SIZE;
			QPoint pixelSize = (QPoint((8 - j), i+1) + tileOffset) * PIXEL_SIZE;

			painter.fillRect(QRect(pixelOffset, pixelSize), brush);
		}
	}

	painter.end();
}

std::vector<WORD> PPU::buildTile(std::vector<BYTE> bytes) {
	std::vector<WORD> tiles(8, 0);
	for (int i = 0; i < tiles.size(); i++) {
		for (int j = 0; j < 8; j++) {
			//extract least significant bits
			WORD bit1 = bytes[2 * i] & 0x1; WORD bit2 = bytes[2 * i + 1] & 0x1;

			//place them in their corresponding positions
			tiles[i] = tiles[i] | (bit1 << 2 * j) | (bit2 << (2 * j + 1));

			//discard them
			bytes[2 * i] = bytes[2 * i] >> 1; bytes[2 * i + 1] = bytes[2 * i + 1] >> 1;
		}
	}

	return tiles;
}
