#include <QGuiApplication>
#include <QApplication>
#include "romstable.h"

int main(int argc, char *argv[])
{
#if defined(Q_OS_WIN)
    QCoreApplication::setAttribute(Qt::AA_EnableHighDpiScaling);
#endif

    QApplication app(argc, argv);
    ROMsTable table;

    table.show();

    return app.exec();
}
