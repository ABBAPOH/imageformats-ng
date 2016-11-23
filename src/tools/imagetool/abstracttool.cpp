#include "abstracttool.h"

#include <QtCore/QCoreApplication>

AbstractTool::~AbstractTool() = default;

void AbstractTool::printUsage(const QCommandLineParser &parser)
{
    const auto name = QString::fromLatin1(id());
    const auto text = parser.helpText().replace(qApp->applicationFilePath(),
                                                QString("%1 %2").arg("imagetool").arg(name));
    printf("%s", text.toLocal8Bit().data());
}
