#include "mainparser.h"
#include "exception.h"

MainParser::MainParser(const DescriptionMap &map) :
    _map(map),
    helpOption(parser.addHelpOption()),
    versionOption(parser.addVersionOption())
{
    parser.setOptionsAfterPositionalArgumentsMode(QCommandLineParser::ParseAsPositionalArguments);
}

/*!
    Processes options. If an error occured, or help, or version is required, prints help and
    throws ExitException, which leads to program exit.
*/
void MainParser::process(const QStringList &arguments)
{
    if (!parser.parse(arguments)) {
        showError(parser.errorText());
        showHelp(1);
    }

    if (parser.isSet(versionOption))
        showVersion();

    const auto positional = parser.positionalArguments();
    if (!positional.isEmpty()) {
        _name = positional.first();
        _arguments = positional;
        if (parser.isSet(helpOption))
            _arguments.insert(1, "--help");
    } else {
        showHelp();
    }
}

void MainParser::showHelp(int code)
{
    auto text = parser.helpText();
    auto lines = text.split("\n");
    lines[0] = "Usage: imagetool [options] command [command options]";
    lines.append("Commands:");
    for (const auto &tool: _map) {
        lines.append(QString("  %1 %2").
                     arg(QString::fromLatin1(tool.first), -10).
                     arg(tool.second));
    }
    lines.append(QString());
    lines.append("Use imagetool command --help to see command options");
    text = lines.join("\n");
    showMessage(text);
    exit(code);
}

void MainParser::showVersion()
{
    showMessage(QCoreApplication::applicationName() + QLatin1Char(' ')
                + QCoreApplication::applicationVersion());
    exit(0);
}

void MainParser::exit(int code)
{
    throw ExitException(code);
}
