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
    QCommandLineParser parser;
    parser.addHelpOption();
    parser.addPositionalArgument(QLatin1String("input"), "Input file");
    parser.parse(arguments);
    auto positionalArguments = parser.positionalArguments();
    if (parser.isSet("help"))
        printUsage(parser, "show");
    if (positionalArguments.size() != 1)
        printUsage(parser, "show");
    _options.insert("input", positionalArguments.at(0));
}

int main(int argc, char *argv[])
{
    QCoreApplication app(argc, argv);

    const auto arguments = app.arguments().mid(1);
    ArgumentsParser parser;
    parser.parse(arguments);

    return 0;
}
