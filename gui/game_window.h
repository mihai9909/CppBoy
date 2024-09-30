#include <QMainWindow>
#include <QWidget>
#include "../emulator/PPU.h"
#include "../emulator/memory.h"

#define WINDOW_HEIGHT_PX 800
#define WINDOW_WIDTH_PX 720

class GameWindow : public QMainWindow
{
    Q_OBJECT

public:
    GameWindow(PPU* ppu, Memory* memory, QWidget* parent = nullptr);
    ~GameWindow();

    void show(QString fileName);

private:
    PPU* ppu;
    Memory* memory;
};
