#include "converttool.h"
#include "exception.h"
#include "toolparser.h"

#include <ImageIO>
#include <QtCore/QCoreApplication>
#include <QDebug>

namespace {

static const char toolId[] = "convert";

struct Options
{
    QString inputFile;
    QString inputMimeType;
    QString outputFile;
    QString outputMimeType;
};

static Options parseOptions(const QStringList &arguments)
{
    ToolParser parser(toolId);
    QCommandLineOption inputTypeOption("input-type", "Input mime type (i.e. image/png)", "mime type");
    QCommandLineOption outputTypeOption("output-type", "Output mime type (i.e. image/png)", "mime type");
    parser.addOption(inputTypeOption);
    parser.addOption(outputTypeOption);
    parser.addPositionalArgument("input", "Input filename", "input");
    parser.addPositionalArgument("output", "Output filename", "output");

    parser.process(arguments);

    const auto positional = parser.positionalArguments();
    if (positional.size() != 2) {
        parser.showError(QString("Incorrect input/output arguments"));
        parser.showHelp(EXIT_FAILURE);
    }

    Options options;
    options.inputFile = positional.at(0);
    options.outputFile = positional.at(1);
    options.inputMimeType = parser.value(inputTypeOption);
    options.outputMimeType = parser.value(outputTypeOption);
    return options;
}

static void convert(const Options &options)
{
    ImageIO io(options.inputFile);
    if (!options.inputMimeType.isEmpty())
        io.setMimeType(options.inputMimeType);
    const auto contents = io.read();
    if (!contents) {
        throw RuntimeError(QString("Can't read image %1: %2").
                           arg(options.inputFile).arg(io.error().errorString()));
    }

    if (!options.outputMimeType.isEmpty())
        io.setMimeType(options.outputMimeType);
    io.setFileName(options.outputFile);
    if (!io.write(*contents)) {
        throw RuntimeError(QString("Can't write image %1: %2").
                           arg(options.outputFile).arg(io.error().errorString()));
    }
}

} // namespace

ConvertTool::ConvertTool()
{
}

QByteArray ConvertTool::id() const
{
    return toolId;
}

QString ConvertTool::decription() const
{
    return qApp->tr("Converts image files", "ImageTool");
}

int ConvertTool::run(const QStringList &arguments)
{
    const auto options = parseOptions(arguments);
    convert(options);
    return 0;
}

