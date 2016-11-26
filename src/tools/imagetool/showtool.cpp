#include "showtool.h"
#include "exception.h"
#include "toolparser.h"

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
    ToolParser parser(id());
    QCommandLineOption listFormatsOption("list-formats", "Shows the list of available formats");
    parser.addOption(listFormatsOption);
    parser.addPositionalArgument("file", "Input filename", "[file]");

    parser.process(arguments);

    if (parser.isSet(listFormatsOption)) {
        showFormatsList();
        return 0;
    }

    const auto positional = parser.positionalArguments();
    if (positional.empty()) {
        parser.showError(QString("File argument missing"));
        parser.showHelp();
    }

    showImageInfo(positional.front());

    return 0;
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
