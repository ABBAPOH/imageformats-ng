#include "abstracttool.h"
#include "showtool.h"
#include "exception.h"

#include <QtCore/QCommandLineParser>
#include <QtCore/QDebug>
#include <QtGui/QGuiApplication>

#include <ImageIO>

#include <map>
#include <memory>

using ToolsMap = std::map<QByteArray, std::unique_ptr<AbstractTool>>;

class ArgumentsParser
{
public:
    enum Mode { Invalid = 0x0, Help = 0x1, Version = 0x2, Run = 0x4 };
    Q_DECLARE_FLAGS(Modes, Mode)

    ArgumentsParser();

    bool parse(const QStringList &arguments);
    void printUsage(const ToolsMap &map);

    Modes mode() const { return _mode; }
    inline QString toolName() { return _name; }
    inline QStringList arguments() const { return _arguments; }

    QStringList unknownOptionNames() const { return parser.unknownOptionNames(); }

private:
    Modes _mode;
    QString _name;
    QStringList _arguments;
    QCommandLineParser parser;
    QCommandLineOption helpOption;
    QCommandLineOption versionOption;
};

ArgumentsParser::ArgumentsParser() :
    helpOption(parser.addHelpOption()),
    versionOption(parser.addVersionOption())
{
    parser.setOptionsAfterPositionalArgumentsMode(QCommandLineParser::ParseAsPositionalArguments);
}

bool ArgumentsParser::parse(const QStringList &arguments)
{
    parser.parse(arguments);

    const auto unknown = parser.unknownOptionNames();
    if (!unknown.isEmpty())
        return false;

    if (parser.isSet(helpOption))
        _mode |= Help;
    if (parser.isSet(versionOption))
        _mode |= Version;

    const auto positional = parser.positionalArguments();
    if (!positional.isEmpty()) {
        _name = positional.first();
        _arguments = positional;
        _mode |= Run;
    }

    return true;
}

void ArgumentsParser::printUsage(const ToolsMap &tools)
{
    auto text = parser.helpText();
    auto lines = text.split("\n");
    lines[0] = "Usage: imagetool [options] command";
    lines.append("Commands:");
    for (const auto &tool: tools) {
        lines.append(QString("  %1 %2").
                     arg(QString::fromLatin1(tool.second->id()), -10).
                     arg(tool.second->decription()));
    }
    lines.append(QString());
    text = lines.join("\n");
    printf("%s", text.toLocal8Bit().data());
}

static ToolsMap CreateTools()
{
    std::unique_ptr<AbstractTool> showTool(new ShowTool);
    ToolsMap result;
    result[showTool->id()] = std::move(showTool);
    return result;
}

int main(int argc, char *argv[])
{
    try {
        QGuiApplication app(argc, argv);
        app.addLibraryPath(app.applicationDirPath() + ImageIO::pluginsDirPath());

        const auto tools = CreateTools();

        ArgumentsParser parser;

        if (!parser.parse(app.arguments())) {
            const auto optionNames = parser.unknownOptionNames();
            const auto message = QString("%1: %2").
                    arg(optionNames.size() > 1 ? "Bad options" : "Bad option")
                    .arg(optionNames.join(", "));
            printf("%s\n", qPrintable(message));
            parser.printUsage(tools);
            return 1;
        }

        if (parser.mode() & ArgumentsParser::Version)
            printf("version 1.0\n");

        if (parser.mode() & ArgumentsParser::Run) {
            const auto toolName = parser.toolName();
            const auto it = tools.find(toolName.toLatin1());
            if (it == tools.end()) {
                printf("%s\n", qPrintable(QString("Unknown command %1").arg(toolName)));
                parser.printUsage(tools);
                return 1;
            }

            if (parser.mode() & ArgumentsParser::Help) {
                it->second->printUsage();
                return 0;
            } else {
                return it->second->run(parser.arguments());
            }
        }

        if (parser.mode() & ArgumentsParser::Help)
            parser.printUsage(tools);

        return 0;
    } catch (const RuntimeError &ex) {
        qWarning() << ex.message();
        return 1;
    } catch (const std::exception &ex) {
        qWarning() << ex.what();
        return 1;
    }
}

