#include "gameboy.h"

GameBoy::GameBoy(PPU* ppu, Memory* memory, QWidget* parent)
    : QMainWindow(parent)
{
    resize(WINDOW_WIDTH_PX, WINDOW_HEIGHT_PX);
    setWindowFlags(Qt::Window | Qt::MSWindowsFixedSizeDialogHint);
    this->ppu = ppu;
    this->memory = memory;
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
