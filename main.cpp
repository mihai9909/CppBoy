#include <QGuiApplication>
#include <QApplication>
#include "gui/romstable.h"

int main(int argc, char *argv[])
{
#if defined(Q_OS_WIN)
    QCoreApplication::setAttribute(Qt::AA_EnableHighDpiScaling);
#endif

    QApplication app(argc, argv);
    HeaderParser headerParser;
    ROMsTable table(headerParser);

    table.show();

    return app.exec();
}
