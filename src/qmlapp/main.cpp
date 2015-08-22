
#include <QtGui/QGuiApplication>
#include <QtQuick/QQuickView>

#include <ImageDocument>
#include <ImageViewItem>

int main(int argc, char *argv[])
{
    QGuiApplication app(argc, argv);

    app.addLibraryPath(app.applicationDirPath() + ImageDocument::pluginsDirPath());

    qmlRegisterType<ImageItem>("com.arch.ImageViewer", 1, 0, "ImageItem");

    QQuickView w;
    w.setSource(QUrl("qrc:/main.qml"));
    w.show();

    return app.exec();
}
