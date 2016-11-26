#include "showtool.h"
#include "exception.h"

#include <ImageIO>
#include <ImageInfoModel>
#include <VariantMapModel>

#include <QtCore/QCoreApplication>
#include <QtCore/QDebug>

QString modelToText(QAbstractTableModel *model)
{
    QStringList result;
    for (int row = 0, rowCount = model->rowCount(); row < rowCount; ++row) {
        const auto keyIndex = model->index(row, 0);
        const auto valueIndex = model->index(row, 1);
        result.append(QString("%1 %2").
                      arg(model->data(keyIndex).toString(), -15).
                      arg(model->data(valueIndex).toString()));
    }
    return result.join("\n");
}

ShowTool::ShowTool()
{
}

QByteArray ShowTool::id() const
{
    return "show";
}

QString ShowTool::decription() const
{
    return qApp->tr("Shows information about image file", "ImageTool");
}

int ShowTool::run(const QStringList &arguments)
{
    QCommandLineParser parser;
    QCommandLineOption helpOption = parser.addHelpOption();
    QCommandLineOption listFormatsOption("list-formats", "Shows the list of available formats");
    parser.addOption(listFormatsOption);
    parser.addPositionalArgument("file", "Input filename", "[file]");

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

    if (parser.isSet(listFormatsOption)) {
        showFormatsList();
        return 0;
    }

    auto positional = parser.positionalArguments();
    if (positional.empty()) {
        printf("%s\n", qPrintable(QString("File argument missing")));
        printUsage(parser);
        return 1;
    }

    showImageInfo(positional.front());

    return 0;
}

void ShowTool::printUsage(const QCommandLineParser &parser)
{
    AbstractTool::printUsage(parser);
}

void ShowTool::showFormatsList() const
{
    for (const auto &formatInfo: ImageIO::supportedImageFormats()) {
        formatInfo.capabilities();
        auto s = QString("%1 %2 %3").
                arg(QString::fromLatin1(formatInfo.name()), -8).
                arg(formatInfo.mimeType().name(), -12).
                arg(formatInfo.capabilitiesString(), -8);
        printf("%s\n", qPrintable(s));
    }
}

void ShowTool::showImageInfo(const QString &filePath) const
{
    ImageIO io(filePath);
    const auto contents = io.read();
    if (!contents) {
        throw RuntimeError(QString("Can't read image %1: %2").
                           arg(filePath).arg(io.error().errorString()));
    }

    ImageInfoModel model;
    model.setImageContents(*contents);
    printf("%s\n", modelToText(&model).toLocal8Bit().data());

    const auto exifMap = contents->exifMeta().toVariantMap();
    if (!exifMap.isEmpty()) {
        printf("\n");
        printf("==== Exif ====\n");
        VariantMapModel exifModel(exifMap);
        printf("%s\n", modelToText(&exifModel).toLocal8Bit().data());
    }
}
