#include "converttool.h"
#include "exception.h"
#include "toolparser.h"

#include <ImageIO>
#include <QtCore/QCoreApplication>
#include <QDebug>

namespace ImageTool {

namespace {

static const char toolId[] = "convert";

struct Options
{
    QString inputFile;
    QString inputMimeType;
    QString outputFile;
    QString outputMimeType;
    QByteArray subType;
};

static Options parseOptions(const QStringList &arguments)
{
    ToolParser parser(toolId);
    QCommandLineOption inputTypeOption(QStringLiteral("input-type"),
                                       ConvertTool::tr("Input mime type (i.e. image/png)"),
                                       QStringLiteral("mime type"));
    QCommandLineOption outputTypeOption(QLatin1String("output-type"),
                                        ConvertTool::tr("Output mime type (i.e. image/png)"),
                                        QStringLiteral("mime type"));
    QCommandLineOption subTypeOption(QStringLiteral("subtype"),
                                     ConvertTool::tr("Output sub type (depends on output format)"),
                                     QStringLiteral("type"));
    parser.addOption(inputTypeOption);
    parser.addOption(outputTypeOption);
    parser.addOption(subTypeOption);
    parser.addPositionalArgument(QStringLiteral("input"),
                                 ConvertTool::tr("Input filename"),
                                 QStringLiteral("input"));
    parser.addPositionalArgument(QStringLiteral("output"),
                                 ConvertTool::tr("Output filename"),
                                 QStringLiteral("output"));

    parser.process(arguments);

    const auto positional = parser.positionalArguments();
    if (positional.size() != 2) {
        parser.showError(ConvertTool::tr("Incorrect input/output arguments"));
        parser.showHelp(EXIT_FAILURE);
    }

    Options options;
    options.inputFile = positional.at(0);
    options.outputFile = positional.at(1);
    options.inputMimeType = parser.value(inputTypeOption);
    options.outputMimeType = parser.value(outputTypeOption);
    options.subType = parser.value(subTypeOption).toLocal8Bit();
    return options;
}

static void convert(const Options &options)
{
    ImageIO io(options.inputFile);
    if (!options.inputMimeType.isEmpty())
        io.setMimeType(options.inputMimeType);
    const auto result = io.read();
    const auto &status = result.first;
    const auto &contents = result.second;
    if (!status) {
        throw RuntimeError(ConvertTool::tr("Can't read image %1: %2").
                           arg(options.inputFile).
                           arg(status.toString()));
    }

    if (!options.outputMimeType.isEmpty())
        io.setMimeType(options.outputMimeType);
    io.setFileName(options.outputFile);
    io.setSubType(options.subType);
    const auto ok = io.write(contents);
    if (!ok) {
        throw RuntimeError(ConvertTool::tr("Can't write image %1: %2").
                           arg(options.outputFile).
                           arg(ok.toString()));
    }
}

} // namespace

/*!
    \class ConvertTool
    This is class implements image converting tool.
*/

/*!
    Constructs a ConvertTool instance.
*/
ConvertTool::ConvertTool() = default;

/*!
    \overload
*/
QByteArray ConvertTool::id() const
{
    return toolId;
}

/*!
    \overload
*/
QString ConvertTool::decription() const
{
    return ConvertTool::tr("Converts image files");
}

/*!
    \overload
*/
int ConvertTool::run(const QStringList &arguments)
{
    const auto options = parseOptions(arguments);
    convert(options);
    return 0;
}

} // namespace ImageTool
