
#include <QtGui/QGuiApplication>
#include <QtQuick/QQuickView>

#include "imageitem.h"

int main(int argc, char *argv[])
{
    QGuiApplication app(argc, argv);

#if defined(Q_OS_LINUX)
    app.addLibraryPath(app.applicationDirPath() + "/../lib/imageviewer/plugins/");
#elif defined(Q_OS_MAC)
    app.addLibraryPath(app.applicationDirPath() + "/../PlugIns/");
#endif

    qmlRegisterType<ImageItem>("com.arch.ImageViewer", 1, 0, "ImageItem");

    QQuickView w;
    w.setSource(QUrl("qrc:/main.qml"));
    w.show();

    return app.exec();
}
