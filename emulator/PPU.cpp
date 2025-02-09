#include "PPU.h"

PPU::PPU(Memory* memory) : memory(memory)
{
	frameBuffer = std::vector<Tile>((VRAM_END - VRAM_START) / BTILE_SIZE);
}

PPU::~PPU() {}

void PPU::updateFrameBuffer() {
	for (int i = VRAM_START; i < VRAM_END; i += BTILE_SIZE) {
		std::vector<BYTE> tileBytes = memory->readByteSequence(i, BTILE_SIZE);
		Tile tile = buildTile(tileBytes);
		int tilePositionInBuffer = ((i - VRAM_START) / BTILE_SIZE);
		frameBuffer[tilePositionInBuffer] = tile;
	}
	renderCallback(frameBuffer);
}

void PPU::displayTileMap() {
	memory->setByte(LCDC, (BG_TILE_MAP));
	BYTE lcdcReg = memory->readByte(LCDC);
	WORD tileMapStart = (lcdcReg & BG_TILE_MAP) != 0 ? TILE_MAP_2_START : TILE_MAP_1_START;


	for (int i = tileMapStart; i < tileMapStart + TILE_MAP_LEN; i++) {
		BYTE tileIndex = memory->readByte(i);
		std::vector<BYTE> tileData = readTile(tileIndex);
		Tile tile = buildTile(tileData);
		int normalizedTilePos = i - tileMapStart;
		/*QPoint screenCoordinates = QPoint(normalizedTilePos % 32, normalizedTilePos / 32) * 8;

		std::vector<QBrush> palette = { QBrush(QColor(8,24,32)), QBrush(QColor(52,104,86)), QBrush(QColor(136,192,112)), QBrush(QColor(224,248,208)) };
		drawTile(tile, screenCoordinates, palette);*/
	}
}

std::vector<BYTE> PPU::readTile(BYTE tileIndex) {
	WORD tileDataLocation;
	BYTE lcdcReg = memory->readByte(LCDC);

	if ((lcdcReg & TILE_MAP_ADDRESSING) != 0) {
		tileDataLocation = BLOCK_0_START + (tileIndex * BTILE_SIZE);
	} else {
		SBYTE signedTileIndex = (SBYTE) tileIndex;
		tileDataLocation = BLOCK_2_START + ((SWORD)signedTileIndex * BTILE_SIZE);
	}
	return memory->readByteSequence(tileDataLocation, BTILE_SIZE);
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

void PPU::setRenderCallback(std::function<void(const std::vector<std::vector<WORD>>&)> callback) {
	renderCallback = callback;
}
