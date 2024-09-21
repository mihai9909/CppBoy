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
    GameWindow gameWindow(new PPU, new Memory);
    ROMsTable table(&headerParser, &gameWindow);

    table.show();

    return app.exec();
}
