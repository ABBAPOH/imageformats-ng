#include "converttool.h"
#include "exception.h"

#include <ImageIO>
#include <QtCore/QCoreApplication>
#include <QDebug>

ConvertTool::ConvertTool()
{

}

QByteArray ConvertTool::id() const
{
    return "convert";
}

QString ConvertTool::decription() const
{
    return qApp->tr("Converts image files", "ImageTool");
}

int ConvertTool::run(const QStringList &arguments)
{
    QCommandLineParser parser;
    QCommandLineOption helpOption = parser.addHelpOption();
    QCommandLineOption inputTypeOption("input-type", "Input mime type (i.e. image/png)", "input-type");
    QCommandLineOption outputTypeOption("output-type", "Output mime type (i.e. image/png)", "output-type");
    parser.addOption(inputTypeOption);
    parser.addOption(outputTypeOption);
    parser.addPositionalArgument("input", "Input filename", "input");
    parser.addPositionalArgument("output", "Output filename", "output");

    if (!parser.parse(arguments)) {
        const auto optionNames = parser.unknownOptionNames();
        if (!optionNames.isEmpty()) {
            const auto message = QString("%1: %2").
                    arg(optionNames.size() > 1 ? "Bad options" : "Bad option")
                    .arg(optionNames.join(", "));
            printf("%s\n", qPrintable(message));
            printUsage(parser);
            return 1;
        }
    }

    if (parser.isSet(helpOption)) {
        printUsage(parser);
        return 0;
    }

    const auto positional = parser.positionalArguments();
    if (positional.size() != 2) {
        printf("%s\n", qPrintable(QString("Incorrect input/output arguments")));
        printUsage(parser);
        return 1;
    }

    Options options;
    options.inputFile = positional.at(0);
    options.outputFile = positional.at(1);
    options.inputMimeType = parser.value(inputTypeOption);
    options.outputMimeType = parser.value(outputTypeOption);
    convert(options);
    return 0;
}

void ConvertTool::convert(const Options &options)
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
