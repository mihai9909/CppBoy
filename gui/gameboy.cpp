#include "gameboy.h"

GameBoy::GameBoy(PPU* ppu, Memory* memory, QWidget* parent)
    : QMainWindow(parent), ppu(ppu), memory(memory)
{
    resize(WINDOW_WIDTH_PX, WINDOW_HEIGHT_PX);
    setWindowFlags(Qt::Window | Qt::MSWindowsFixedSizeDialogHint);
    setCentralWidget(ppu);
}

GameBoy::~GameBoy()
{
    delete ppu;
    delete memory;
}

void GameBoy::show(QString fileName) {
    memory->loadMemory(fileName);
    QMainWindow::show();
}
