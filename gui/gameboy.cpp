#include "gameboy.h"

GameBoy::GameBoy(GameBoyCore* core, QWidget* parent)
    : QMainWindow(parent), core(core), gameWindow(new GameWindow(this)) {
    core->setRenderCallback([this](const std::vector<std::vector<WORD>>& frameBuffer) { renderScreen(frameBuffer); });
    resize(WINDOW_WIDTH_PX, WINDOW_HEIGHT_PX);
    setWindowFlags(Qt::Window | Qt::MSWindowsFixedSizeDialogHint);
    setCentralWidget(gameWindow);
}

GameBoy::~GameBoy() { }

void GameBoy::show() {
    QMainWindow::show();
}

void GameBoy::closeEvent(QCloseEvent* event) {
    delete core;
    delete gameWindow;
    event->accept();
}

void GameBoy::renderScreen(const std::vector<std::vector<WORD>>& frameBuffer) {
    gameWindow->setFrameBuffer(frameBuffer);
}

void GameBoy::keyPressEvent(QKeyEvent* event) {
    core->handleInput(event->key());
}
