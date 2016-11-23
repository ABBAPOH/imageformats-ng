#include "showtool.h"

#include <QtCore/QCoreApplication>
#include <QtCore/QDebug>

QByteArray ShowTool::id() const
{
    return "show";
}

QString ShowTool::decription() const
{
    return qApp->tr("ImageTool", "Shows information about image file");
}

int ShowTool::run(const QStringList &arguments)
{
    qDebug() << "ShowTool::run" << arguments;
    return 0;
}

void ShowTool::printHelp()
{
    qDebug() << "ShowTool::printHelp";
}
