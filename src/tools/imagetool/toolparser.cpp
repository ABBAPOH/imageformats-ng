#include "toolparser.h"
#include "exception.h"

ToolParser::ToolParser(const QByteArray &toolName) :
    _toolId(toolName),
    helpOption(addHelpOption())
{
    if (_toolId.isEmpty())
        throw RuntimeError("Tool name is empty");
}

void ToolParser::process(const QStringList &arguments)
{
    if (!parse(arguments)) {
        showError(errorText());
        showHelp(EXIT_FAILURE);
    }

    if (isSet(helpOption))
        showHelp(EXIT_SUCCESS);
}

void ToolParser::showHelp(int code)
{
    const auto text = helpText().replace(qApp->applicationFilePath(),
                                         QString("%1 %2").arg("imagetool").arg(QString::fromLatin1(_toolId)));
    showMessage(text);
    exit(code);
}

void ToolParser::showMessage(const QString &message)

{
    fputs(qPrintable(message + "\n"), stdout);
}

void ToolParser::showError(const QString &message)
{
    fputs(qPrintable(message + "\n"), stderr);
}

void ToolParser::exit(int code)
{
    throw ExitException(code);
}
