#include "mainwindow.h"

#include <ImageIO>
#include <ImageViewItem>

#include <QtCore/QCommandLineParser>
#include <QtCore/QCommandLineOption>
#include <QtCore/QMimeDatabase>
#include <QtQuick/QQuickView>
#include <QtWidgets/QApplication>

int main(int argc, char *argv[])
{
    QApplication app(argc, argv);
    app.addLibraryPath(app.applicationDirPath() + ImageIO::pluginsDirPath());

    // this is used to preload QMimeDatabase to speedup loading plugins
    QMimeDatabase().mimeTypeForName("application/octet-stream");

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
        auto args = app.arguments();
        MainWindow w;
        if (args.size() == 2)
            w.openDocument(QUrl::fromLocalFile(args.at(1)));

        w.show();

        return app.exec();
    }
}
