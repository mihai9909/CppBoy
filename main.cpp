#include <QGuiApplication>
#include <QApplication>
#include <QOpenGLWidget>
#include <QOpenGLFunctions>
#include "gui/romstable.h"
#include "emulator/ppu.h"
#include "emulator/memory.h"

int main(int argc, char *argv[])
{
#if defined(Q_OS_WIN)
    QCoreApplication::setAttribute(Qt::AA_EnableHighDpiScaling);
#endif

    QApplication app(argc, argv);
    HeaderParser headerParser;
    Memory* gameboyMem = new Memory();
    PPU* gameboyPPU = new PPU(gameboyMem);
    GameBoy gameBoy(gameboyPPU, gameboyMem);
    ROMsTable table(&headerParser, &gameBoy);

    table.show();

    return app.exec();
}
