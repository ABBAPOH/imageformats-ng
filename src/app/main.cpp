#include "mainwindow.h"

#include <QtWidgets/QApplication>

int main(int argc, char *argv[])
{
    QApplication app(argc, argv);

#ifdef Q_OS_LINUX
    app.addLibraryPath(app.applicationDirPath() + "/../lib/imageviewer/plugins/");
#elif Q_OS_MAC
    app.addLibraryPath(app.applicationDirPath() + "/../PlugIns/");
#endif

    MainWindow w;
    w.show();

    return app.exec();
}
