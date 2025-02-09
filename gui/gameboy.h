#pragma once

#include <QMainWindow>
#include <QWidget>
#include <QKeyEvent>
#include "game_window.h"
#include "../emulator/gameboy_core.h"

#define WINDOW_HEIGHT_PX 512
#define WINDOW_WIDTH_PX 512

class GameBoy : public QMainWindow {
    Q_OBJECT

public:
    GameBoy(GameBoyCore* core, QWidget* parent = nullptr);
    ~GameBoy();

    void show();

protected:
    void closeEvent(QCloseEvent* event) override;
    void keyPressEvent(QKeyEvent* event) override;

private:
    void renderScreen(const std::vector<std::vector<WORD>>& frameBuffer);

    GameBoyCore* core;
    GameWindow* gameWindow;
};
