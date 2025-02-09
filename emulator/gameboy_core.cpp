#include "gameboy_core.h"

GameBoyCore::GameBoyCore(PPU* ppu, Memory* memory, GBZ80* gbz80)
    : ppu(ppu), memory(memory), gbz80(gbz80), cartridge(nullptr), running(true) {
}

GameBoyCore::~GameBoyCore() {
    stop();
    delete gbz80;
    delete ppu;
    delete memory;
}

void GameBoyCore::mapBootROM() {
    for (int i = 0x00; i < 0x100; i++) {
        memory->setByte(i, bootROM[i]);
    }
}

void GameBoyCore::setJoypadReg(BYTE key) {
    memory->setByte(JOYPAD, key);
    requestInterrupt(JOYPAD_INT);
}

void GameBoyCore::run(Cartridge* cartridge) {
    this->cartridge = cartridge;
    memory->loadCartridge(cartridge);
    mapBootROM();

    QThreadPool::globalInstance()->start([this]() {
        while (running) {
            gbz80->execute();
            ppu->updateFrameBuffer();
        }
    });
}

void GameBoyCore::handleInput(const int key) {
    switch ((char)(key))
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

void GameBoyCore::requestInterrupt(BYTE interrupt) {
    BYTE intFlag = memory->readByte(IF);
    memory->setByte(IF, (intFlag | JOYPAD_INT));
}

void GameBoyCore::setRenderCallback(std::function<void(const std::vector<std::vector<WORD>>&)> callback) {
    ppu->setRenderCallback(callback);
}

void GameBoyCore::stop() {
    running = false;
    QThreadPool::globalInstance()->waitForDone();
}
