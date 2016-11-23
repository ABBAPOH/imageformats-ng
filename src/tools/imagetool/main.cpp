#include "abstracttool.h"
#include "showtool.h"

#include <QtCore/QCoreApplication>
#include <QtCore/QCommandLineParser>
#include <QtCore/QDebug>

#include <map>
#include <memory>

class ArgumentsParser
{
public:
    enum class Mode { Invalid, Help, Show };
    void parse(const QStringList &arguments);
    void printUsage();
    void printUsage(QCommandLineParser &parser, const QString &name);

    Mode mode() const;
    QVariantHash options() const;

private:
    void parseShow(const QStringList &arguments);

private:
    Mode _mode { Mode::Invalid };
    QVariantHash _options;
};

void ArgumentsParser::parse(const QStringList &arguments)
{
    if (arguments.empty() || arguments.at(0) == QLatin1String("--help")) {
        printUsage();
        exit(0);
    }

    if (arguments[0] == QLatin1String("show")) {
        parseShow(arguments);
    } else {
        qDebug() << "usage: ./imagetool show";
    }
}

void ArgumentsParser::printUsage()
{
    qDebug() << "usage: ./imagetool [command] [command parameters]";
    qDebug() << "Commands:";
    qDebug() << "    show\t Shows the info about the image. This is the default command.";
}

void ArgumentsParser::printUsage(QCommandLineParser &parser, const QString &name)
{
    const auto text = parser.helpText().replace(qApp->applicationFilePath(),
                                                QString("%1 %2").arg("imagetool").arg(name));
    printf("%s", text.toLocal8Bit().data());
    exit(0);
}

void ArgumentsParser::parseShow(const QStringList &arguments)
{
    _mode = Mode::Show;
}

using ToolsMap = std::map<QByteArray, std::unique_ptr<AbstractTool>>;
static ToolsMap CreateTools()
{
    std::unique_ptr<AbstractTool> showTool(new ShowTool);
    ToolsMap result;
    result[showTool->id()] = std::move(showTool);
    return result;
}

int main(int argc, char *argv[])
{
    QCoreApplication app(argc, argv);

    const auto tools = CreateTools();

    const auto arguments = app.arguments();
//    ArgumentsParser parser;
//    parser.parse(arguments);

    QCommandLineParser parser;
    parser.setOptionsAfterPositionalArgumentsMode(QCommandLineParser::ParseAsPositionalArguments);
    const auto helpOption = parser.addHelpOption();
    const auto versionOption = parser.addVersionOption();
    parser.parse(arguments);
    auto positionalArguments = parser.positionalArguments();
    auto toolName = positionalArguments.isEmpty() ? QString() : positionalArguments.at(0);
    const auto it = tools.find(toolName.toLatin1());
    if (parser.isSet(helpOption)) {
        if (toolName.isEmpty()) {
            qDebug() << "usage";
            return 0;
        } else {
            if (it == tools.end()) {
                qDebug() << "usage";
                return 0;
            } else {
                it->second->printHelp();
                return 0;
            }
        }
    }
    if (parser.isSet(versionOption)) {
        qDebug() << "version 1.0";
        return 0;
    }

    if (it == tools.end()) {
        qDebug() << "usage";
        return 0;
    }

    it->second->run(positionalArguments);

    return 0;
}
