#include "game_window.h"

GameWindow::GameWindow(PPU* ppu, Memory* memory, QWidget* parent)
    : QMainWindow(parent)
{
    resize(WINDOW_HEIGHT_PX, WINDOW_WIDTH_PX);
    setWindowFlags(Qt::Window | Qt::MSWindowsFixedSizeDialogHint);
    this->ppu = ppu;
    this->memory = memory;
    setCentralWidget(ppu);
}

GameWindow::~GameWindow()
{
    delete ppu;
    delete memory;
}

void GameWindow::show(QString fileName) {
    memory->loadMemory(fileName);
    QMainWindow::show();
}
