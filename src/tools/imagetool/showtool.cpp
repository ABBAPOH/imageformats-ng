#include "showtool.h"
#include "exception.h"
#include "toolparser.h"

#include <ImageIO>
#include <ImageInfoModel>
#include <VariantMapModel>

#include <QtCore/QCoreApplication>
#include <QtCore/QDebug>

namespace {

static const char toolId[] = "show";

struct Options
{
    bool listFormats {false};
    bool showInfo {false};
    QString fileName;
};

Options parseOptions(const QStringList &arguments)
{
    ToolParser parser(toolId);
    QCommandLineOption listFormatsOption("list-formats", "Shows the list of available formats");
    parser.addOption(listFormatsOption);
    parser.addPositionalArgument("file", "Input filename", "[file]");

    parser.process(arguments);

    Options result;
    result.listFormats = parser.isSet(listFormatsOption);

    const auto positional = parser.positionalArguments();
    if (positional.size() > 1) {
        parser.showError(QString("Too many file arguments"));
        parser.showHelp(EXIT_FAILURE);
    } else if (positional.size() == 0) {
        if (!result.listFormats) {
            parser.showError(QString("File argument missing"));
            parser.showHelp(EXIT_FAILURE);
        }
    } else {
        result.showInfo = true;
        result.fileName = positional.at(0);
    }

    return result;
}

static inline void showMessage(const QString &message)
{
    ToolParser::showMessage(message);
}

static QString modelToText(QAbstractTableModel *model)
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

static void showFormatsList()
{
    for (const auto &formatInfo: ImageIO::supportedImageFormats()) {
        formatInfo.capabilities();
        const auto infoString = QString("%1 %2 %3").
                arg(QString::fromLatin1(formatInfo.name()), -8).
                arg(formatInfo.mimeType().name(), -12).
                arg(formatInfo.capabilitiesString(), -8);
        showMessage(infoString);
    }
}

static void showImageInfo(const QString &filePath)
{
    ImageIO io(filePath);
    const auto contents = io.read();
    if (!contents) {
        throw RuntimeError(QString("Can't read image %1: %2").
                           arg(filePath).arg(io.error().errorString()));
    }

    ImageInfoModel model;
    model.setImageContents(*contents);
    ToolParser::showMessage(modelToText(&model));

    const auto exifMap = contents->exifMeta().toVariantMap();
    if (!exifMap.isEmpty()) {
        showMessage(QString());
        showMessage("==== Exif ====\n");
        VariantMapModel exifModel(exifMap);
        showMessage(modelToText(&exifModel));
    }
}

} // namespace

ShowTool::ShowTool()
{
}

QByteArray ShowTool::id() const
{
    return toolId;
}

QString ShowTool::decription() const
{
    return qApp->tr("Shows information about image file", "ImageTool");
}

int ShowTool::run(const QStringList &arguments)
{
    const auto options = parseOptions(arguments);

    if (options.listFormats)
        showFormatsList();
    if (options.showInfo)
        showImageInfo(options.fileName);

    return 0;
}
