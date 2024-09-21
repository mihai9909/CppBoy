#include <QMainWindow>
#include <QWidget>
#include "../emulator/PPU.h"
#include "../emulator/memory.h"

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
