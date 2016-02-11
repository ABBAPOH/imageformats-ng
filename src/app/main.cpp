#include "mainwindow.h"

#include <ImageIO>
#include <ImageViewItem>

#include <QtCore/QCommandLineParser>
#include <QtCore/QCommandLineOption>
#include <QtQuick/QQuickView>
#include <QtWidgets/QApplication>

int main(int argc, char *argv[])
{
    QApplication app(argc, argv);
    app.addLibraryPath(app.applicationDirPath() + ImageIO::pluginsDirPath());

    QCommandLineParser parser;
    QCommandLineOption useQmlOption("qml", QCoreApplication::translate("main", "Use qml instead of widgets"));
    parser.addOption(useQmlOption);
    parser.process(app);

    if (parser.isSet(useQmlOption)) {
        qmlRegisterType<ImageViewItem>("com.arch.ImageViewer", 1, 0, "ImageView");

        QQuickView w;
        w.setSource(QUrl("qrc:/main.qml"));
        w.show();

        return app.exec();
    } else {
        MainWindow w;
        w.show();

        return app.exec();
    }
}
