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

ShowTool::ShowTool() :
    helpOption(parser.addHelpOption())
{
    parser.addPositionalArgument("file", "Input filename");
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
    parser.parse(arguments);

    const auto unknown = parser.unknownOptionNames();
    if (!unknown.isEmpty())
        throw BadOption(unknown);

    if (parser.isSet(helpOption)) {
        printUsage();
        return 0;
    }

    auto positional = parser.positionalArguments();
    if (positional.empty())
        return 0;

    auto fileName = positional.front();
    ImageIO io(fileName);
    auto contents = io.read();
    if (!contents) {
        qWarning() << "Can't read image" << fileName << io.error().errorString();
        return 1;
    }
    ImageInfoModel model;
    model.setImageContents(*contents);
    printf("%s\n", modelToText(&model).toLocal8Bit().data());

    printf("==== Exif ====\n");
    VariantMapModel exifModel(contents->exifMeta().toVariantMap());
    printf("%s\n", modelToText(&exifModel).toLocal8Bit().data());

    return 0;
}

void ShowTool::printUsage()
{
    AbstractTool::printUsage(parser);
}
