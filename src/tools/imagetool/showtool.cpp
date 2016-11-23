#include "showtool.h"

#include <QtCore/QCoreApplication>
#include <QtCore/QDebug>

ShowTool::ShowTool() :
    helpOption(parser.addHelpOption())
{
    parser.addPositionalArgument("file", "Input filename");
}

QByteArray ShowTool::id() const
{
    return "show";
}

QString ShowTool::decription() const
{
    return qApp->tr("Shows information about image file", "ImageTool");
}

int ShowTool::run(const QStringList &arguments)
{
    parser.parse(arguments);
    if (parser.isSet(helpOption)) {
        printUsage();
        return 0;
    }
    return 0;
}

void ShowTool::printUsage()
{
    AbstractTool::printUsage(parser);
}
