#include "mainwindow.h"

#include <QtWidgets/QApplication>

int main(int argc, char *argv[])
{
    QApplication app(argc, argv);

    app.addLibraryPath(app.applicationDirPath() + ImageDocument::pluginsDirPath());

    MainWindow w;
    w.show();

    return app.exec();
}
