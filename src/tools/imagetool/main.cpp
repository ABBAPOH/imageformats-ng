#include "abstracttool.h"
#include "converttool.h"
#include "exception.h"
#include "showtool.h"
#include "toolparser.h"

#include <QtCore/QCommandLineParser>
#include <QtCore/QDebug>
#include <QtGui/QGuiApplication>

#include <ImageIO>

#include <map>
#include <memory>

using ToolsMap = std::map<QByteArray, std::unique_ptr<AbstractTool>>;

static inline void showMessage(const QString &message)
{
    ToolParser::showMessage(message);
}

static inline void showError(const QString &message)
{
    ToolParser::showError(message);
}

class ArgumentsParser
{
public:
    explicit ArgumentsParser(const ToolsMap &map);

    void process(const QStringList &arguments);
    Q_NORETURN void showHelp();
    Q_NORETURN void showVersion();

    inline QString toolName() { return _name; }
    inline QStringList arguments() const { return _arguments; }

private:
    Q_NORETURN void exit(int code = 0);

private:
    const ToolsMap &_map;
    QCommandLineParser parser;
    QCommandLineOption helpOption;
    QCommandLineOption versionOption;
    QString _name;
    QStringList _arguments;
};

ArgumentsParser::ArgumentsParser(const ToolsMap &map) :
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
void ArgumentsParser::process(const QStringList &arguments)
{
    if (!parser.parse(arguments)) {
        showError(parser.errorText());
        showHelp();
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
        if (parser.isSet(helpOption))
            showHelp();
    }
}

void ArgumentsParser::showHelp()
{
    const ToolsMap &tools = _map;
    auto text = parser.helpText();
    auto lines = text.split("\n");
    lines[0] = "Usage: imagetool [options] command [command options]";
    lines.append("Commands:");
    for (const auto &tool: tools) {
        lines.append(QString("  %1 %2").
                     arg(QString::fromLatin1(tool.second->id()), -10).
                     arg(tool.second->decription()));
    }
    lines.append(QString());
    lines.append("Use imagetool command --help to see command options");
    text = lines.join("\n");
    showMessage(text);
    exit(0);
}

void ArgumentsParser::showVersion()
{
    showMessage(QCoreApplication::applicationName() + QLatin1Char(' ')
                + QCoreApplication::applicationVersion());
    exit(0);
}

void ArgumentsParser::exit(int code)
{
    throw ExitException(code);
}

static ToolsMap CreateTools()
{
    std::unique_ptr<AbstractTool> showTool(new ShowTool);
    std::unique_ptr<AbstractTool> convertTool(new ConvertTool);
    ToolsMap result;
    result[showTool->id()] = std::move(showTool);
    result[convertTool->id()] = std::move(convertTool);
    return result;
}

int main(int argc, char *argv[])
{
    try {
        QGuiApplication app(argc, argv);
        app.setApplicationName("imagetool");
        app.setApplicationVersion("1.0");
        app.addLibraryPath(app.applicationDirPath() + ImageIO::pluginsDirPath());

        const auto tools = CreateTools();

        ArgumentsParser parser(tools);
        parser.process(app.arguments());

        const auto toolName = parser.toolName();
        const auto it = tools.find(toolName.toLatin1());
        if (it == tools.end()) {
            showError(QString("Unknown command %1").arg(toolName));
            parser.showHelp();
        }

        return it->second->run(parser.arguments());
    } catch (const ExitException &ex) {
        return ex.code();
    } catch (const RuntimeError &ex) {
        qWarning() << ex.message();
        return 1;
    } catch (const std::exception &ex) {
        qWarning() << ex.what();
        return 1;
    }
}

