#include "mainwindow.h"

#include <QtWidgets/QApplication>
#include <QDebug>

int main(int argc, char *argv[])
{
    QApplication app(argc, argv);

    MainWindow w;
    w.show();

    if (QTypeInfo<QImage>::isComplex)
        qDebug() << "complex";
    if (QTypeInfo<QImage>::isDummy)
        qDebug() << "dummy";
    if (QTypeInfo<QImage>::isIntegral)
        qDebug() << "integral";
    if (QTypeInfo<QImage>::isLarge)
        qDebug() << "large";
    if (QTypeInfo<QImage>::isPointer)
        qDebug() << "pointer";
    if (QTypeInfo<QImage>::isStatic)
        qDebug() << "static";

    return app.exec();
}
