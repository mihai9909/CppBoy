#include "game_window.h"

GameWindow::GameWindow(PPU* ppu, Memory* memory, QWidget* parent)
    : QMainWindow(parent)
{
    this->ppu = ppu;
    this->memory = memory;
    setCentralWidget(ppu);
}

GameWindow::~GameWindow()
{
    delete ppu;
}

void GameWindow::show(QString fileName) {
    memory->loadMemory(fileName);
    QMainWindow::show();
}
