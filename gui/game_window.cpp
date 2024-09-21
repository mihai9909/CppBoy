#include "game_window.h"

GameWindow::GameWindow(PPU* ppu, QWidget* parent)
    : QMainWindow(parent)
{
    this->ppu = ppu;
    setCentralWidget(ppu);
}

GameWindow::~GameWindow()
{
    delete ppu;
}

void GameWindow::show(QString fileName) {
    // TODO: load ROM contents into PPU
    QMainWindow::show();
}
