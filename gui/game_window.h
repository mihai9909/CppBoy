#include <QMainWindow>
#include <QWidget>
#include "../emulator/PPU.h"

class GameWindow : public QMainWindow
{
    Q_OBJECT

public:
    GameWindow(PPU* ppu, QWidget* parent = nullptr);
    ~GameWindow();

    void show(QString fileName);

private:
    PPU* ppu;
};
