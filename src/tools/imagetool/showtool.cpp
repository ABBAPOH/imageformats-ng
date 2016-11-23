#include "showtool.h"

#include <QtCore/QCoreApplication>

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
    return 0;
}
