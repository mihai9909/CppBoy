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

void GameBoy::show(Cartridge* cartridge) {
    this->cartridge = cartridge;
    memory->loadCartridge(cartridge);
    QMainWindow::show();
}

void GameBoy::closeEvent(QCloseEvent* event) {
    memory->flush();
}

void GameBoy::keyPressEvent(QKeyEvent* event) {
    switch ((char)(event->key()))
    {
    case 'A': setJoypadReg(LEFT); return;
    case 'S': setJoypadReg(DOWN); return;
    case 'D': setJoypadReg(RIGHT); return;
    case 'W': setJoypadReg(UP); return;
    case 'P': setJoypadReg(SELECT); return;
    case 'O': setJoypadReg(START); return;
    case 'L': setJoypadReg(A); return;
    case 'K': setJoypadReg(B); return;
    default:
        break;
    }
}

void GameBoy::setJoypadReg(BYTE key) {
    memory->setByte(JOYPAD, key);
    requestInterrupt(JOYPAD_INT);
}

void GameBoy::requestInterrupt(BYTE interrupt) {
    BYTE intFlag = memory->readByte(IF);
    memory->setByte(IF, (intFlag | JOYPAD_INT));
}
