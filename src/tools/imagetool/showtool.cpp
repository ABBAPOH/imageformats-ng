#include "showtool.h"
#include "exception.h"
#include "toolparser.h"

#include <ImageIO>
#include <ImageInfoModel>
#include <VariantMapModel>

#include <QtCore/QDebug>

namespace ImageTool {

namespace {

static const char toolId[] = "show";

struct Options
{
    bool listFormats {false};
    bool showInfo {false};
    QString fileName;
};

static Options parseOptions(const QStringList &arguments)
{
    ToolParser parser(toolId);
    QCommandLineOption listFormatsOption(QStringLiteral("list-formats"),
                                         ShowTool::tr("Shows the list of available formats"));
    parser.addOption(listFormatsOption);
    parser.addPositionalArgument(QStringLiteral("file"),
                                 ShowTool::tr("Input filename"),
                                 QStringLiteral("[file]"));

    parser.process(arguments);

    Options result;
    result.listFormats = parser.isSet(listFormatsOption);

    const auto positional = parser.positionalArguments();
    if (positional.size() > 1) {
        parser.showError(ShowTool::tr("Too many file arguments"));
        parser.showHelp(EXIT_FAILURE);
    } else if (positional.size() == 0) {
        if (!result.listFormats) {
            parser.showError(ShowTool::tr("File argument missing"));
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
        result.append(ShowTool::tr("%1 %2").
                      arg(model->data(keyIndex).toString(), -15).
                      arg(model->data(valueIndex).toString()));
    }
    return result.join("\n");
}

static void showFormatInfo(const ImageFormatInfo &formatInfo, const QByteArray &subType)
{
    const auto infoString = ShowTool::tr("%1 %2 %3 %4 %5").
            arg(QString::fromLatin1(formatInfo.name()), -8).
            arg(formatInfo.mimeType().name(), -12).
            arg(QString::fromLatin1(subType), -8).
            arg(formatInfo.capabilitiesString(), -12).
            arg(ImageOptions::optionsToString(formatInfo.supportedOptions(subType)));
    showMessage(infoString);
}

static void showFormatsList()
{
    const auto lessThan = [](const ImageFormatInfo &lhs, const ImageFormatInfo &rhs)
    {
        return lhs.name() < rhs.name();
    };
    auto formats = ImageIO::supportedImageFormats();
    std::sort(formats.begin(), formats.end(), lessThan);

    const auto headerString = ShowTool::tr("%1 %2 %3 %4 %5").
            arg(ShowTool::tr("Name"), -8).
            arg(ShowTool::tr("MimeType"), -12).
            arg(ShowTool::tr("SubType"), -8).
            arg(ShowTool::tr("Capabilities"), -12).
            arg(ShowTool::tr("Options"));
    showMessage(headerString);

    for (const auto &formatInfo: formats) {
        const auto subTypes = formatInfo.supportedSubTypes();
        if (subTypes.isEmpty()) {
            showFormatInfo(formatInfo, QByteArray());
        } else {
            for (auto subType : formatInfo.supportedSubTypes()) {
                showFormatInfo(formatInfo, subType);
            }
        }
    }
}

static void showImageInfo(const QString &filePath)
{
    ImageIO io(filePath);
    const auto result = io.read();
    if (!result.first) {
        throw RuntimeError(ShowTool::tr("Can't read image %1: %2").
                           arg(filePath).arg(result.first.toString()));
    }

    const auto &contents = result.second;
    ImageInfoModel model;
    model.setImageContents(contents);
    ToolParser::showMessage(modelToText(&model));

    const auto exifMap = contents.exifMeta().toVariantMap();
    if (!exifMap.isEmpty()) {
        showMessage(QString());
        showMessage(ShowTool::tr("==== Exif ===="));
        VariantMapModel exifModel(exifMap);
        showMessage(modelToText(&exifModel));
    }
}

} // namespace

/*!
    \class ShowTool
    This is class implements image showing tool.
*/

/*!
    Constructs a ShowTool instance.
*/
ShowTool::ShowTool() = default;

/*!
    \overload
*/
QByteArray ShowTool::id() const
{
    return toolId;
}

/*!
    \overload
*/
QString ShowTool::decription() const
{
    return ShowTool::tr("Shows information about image file", "ImageTool");
}

/*!
    \overload
*/
int ShowTool::run(const QStringList &arguments)
{
    const auto options = parseOptions(arguments);

    if (options.listFormats)
        showFormatsList();
    if (options.showInfo)
        showImageInfo(options.fileName);

    return 0;
}

} // namespace ImageTool
