#include <QMainWindow>
#include <QWidget>
#include "../emulator/PPU.h"
#include "../emulator/cartridge.h"

#define WINDOW_HEIGHT_PX 720
#define WINDOW_WIDTH_PX 800

class GameBoy : public QMainWindow
{
    Q_OBJECT

public:
    GameBoy(PPU* ppu, Memory* memory, QWidget* parent = nullptr);
    ~GameBoy();

    void show(Cartridge* cartridge);

private:
    PPU* ppu;
    Memory* memory;
    Cartridge* cartridge;
};
