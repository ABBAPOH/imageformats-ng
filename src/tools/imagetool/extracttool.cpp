#include "extracttool.h"
#include "exception.h"
#include "toolparser.h"

#include <ImageIO>
#include <ImageResource>

#include <QtCore/QDebug>
#include <QtCore/QDir>
#include <QtCore/QFileInfo>
#include <QtCore/QMimeDatabase>
#include <QtCore/QJsonDocument>
#include <QtCore/QJsonObject>

namespace ImageTool {

namespace {

static const char toolId[] = "extract";
static const QDir::Filters filters = QDir::AllEntries
        | QDir::Hidden
        | QDir::System
        | QDir::NoDotAndDotDot;

struct Options
{
    QString inputFile;
    QString inputMimeType;
    QString outputFolder;
    QString outputMimeType;
    QByteArray subType;
    bool force {false};
};

static Options parseOptions(const QStringList &arguments)
{
    ToolParser parser(toolId);
    QCommandLineOption inputTypeOption(QStringLiteral("input-type"),
                                       ExtractTool::tr("Input mime type (i.e. image/png)"),
                                       QStringLiteral("mime type"));
    QCommandLineOption outputTypeOption(QLatin1String("output-type"),
                                        ExtractTool::tr("Output mime type (i.e. image/png)"),
                                        QStringLiteral("mime type"));
    QCommandLineOption subTypeOption(QStringLiteral("subtype"),
                                     ExtractTool::tr("Output sub type (depends on output format)"),
                                     QStringLiteral("type"));

    QCommandLineOption forceOption({QStringLiteral("f"), QStringLiteral("force")},
                                   ExtractTool::tr("Overwrite existing dir"));
    parser.addOption(inputTypeOption);
    parser.addOption(outputTypeOption);
    parser.addOption(subTypeOption);
    parser.addOption(forceOption);
    parser.addPositionalArgument(QStringLiteral("input"),
                                 ExtractTool::tr("Input filename"),
                                 QStringLiteral("input"));
    parser.addPositionalArgument(QStringLiteral("output"),
                                 ExtractTool::tr("Output folder"),
                                 QStringLiteral("output"));

    parser.process(arguments);

    const auto positional = parser.positionalArguments();
    if (positional.size() != 2) {
        parser.showError(ExtractTool::tr("Incorrect input/output arguments"));
        parser.showHelp(EXIT_FAILURE);
    }

    Options options;
    options.inputFile = positional.at(0);
    options.outputFolder = positional.at(1);
    options.inputMimeType = parser.value(inputTypeOption);
    options.outputMimeType = parser.value(outputTypeOption);
    options.subType = parser.value(subTypeOption).toLocal8Bit();
    options.force = parser.isSet(forceOption);

    return options;
}

static void removePath(const QString &path)
{
    QFileInfo info(path);
    if (info.isDir()) {
        QDir dir(path);
        for (auto entry : dir.entryList(filters)) {
            removePath(dir.absoluteFilePath(entry));
        }
        dir.cdUp();
        if (!dir.rmdir(info.fileName())) {
            throw RuntimeError(ExtractTool::tr("Can't remove dir %1").
                               arg(path));
        }
    } else if (info.isFile()) {
        if (!QFile::remove(path)) {
            throw RuntimeError(ExtractTool::tr("Can't remove file %1").
                               arg(path));
        }
    }
}

static void extractHeader(const ImageHeader &header, const QString &path)
{
    QJsonObject object;
    object["type"] = ImageResource::typeToString(header.type());
//    object["imageFormat"] = header.imageFormat();
    object["width"] = header.width();
    object["heigth"] = header.height();
    object["depth"] = header.depth();
    object["name"] = header.name();
    object["imageCount"] = header.imageCount();
    object["hasMipmaps"] = header.hasMipmaps();
    object["frameDelay"] = header.frameDelay();
    object["loopCount"] = header.loopCount();

    QJsonDocument doc(object);
    const auto json = doc.toJson();
    QFile file(path + "/header.json");
    if (!file.open(QIODevice::WriteOnly)) {
        throw RuntimeError(ExtractTool::tr("Can't open %1 for writing").
                           arg(file.fileName()));
    }

    qint64 totalWritten = 0;
    do {
        const auto written = file.write(json.mid(totalWritten));
        if (written == -1) {
            throw RuntimeError(ExtractTool::tr("Can't write data to %1").
                               arg(file.fileName()));
        }
        totalWritten += written;
    } while (totalWritten < json.size());
}

static void extractData(const QImage &image,
                        const QString &path,
                        const QMimeType &mimeType)
{
    const auto suffix = !mimeType.suffixes().empty()
            ? QStringLiteral(".") + mimeType.suffixes().first()
            : QStringLiteral("");
    QDir dir(QFileInfo(path).absolutePath());
    dir.mkpath(".");
    ImageIO io(path + suffix, mimeType);
//    io.setSubType(subType);
    const auto ok = io.write(ImageContents(image));
    if (!ok) {
        throw RuntimeError(ExtractTool::tr("Can't write image %1: %2").
                           arg(io.fileName()).
                           arg(ok.toString()));
    }
}

static void extractData(const CubeTexture &image,
                        const QString &path,
                        const QMimeType &mimeType)
{
    extractData(image.side(CubeTexture::Side::NegativeX),
                path + QStringLiteral("/NegativeX"),
                mimeType);
    extractData(image.side(CubeTexture::Side::PositiveX)
                , path + QStringLiteral("/PositiveX"),
                mimeType);
    extractData(image.side(CubeTexture::Side::NegativeY),
                path + QStringLiteral("/NegativeY"),
                mimeType);
    extractData(image.side(CubeTexture::Side::PositiveY),
                path + QStringLiteral("/PositiveY"),
                mimeType);
    extractData(image.side(CubeTexture::Side::NegativeZ),
                path + QStringLiteral("/NegativeZ"),
                mimeType);
    extractData(image.side(CubeTexture::Side::PositiveZ),
                path + QStringLiteral("/PositiveZ"),
                mimeType);
}

static void extractData(const VolumeTexture &image,
                        const QString &path,
                        const QMimeType &mimeType)
{
    for (int depth = 0; depth < image.depth(); ++depth) {
        extractData(image.slice(depth),
                    path + QStringLiteral("/slice %1").arg(depth),
                    mimeType);
    }
}

static void extractData(const ImageContents &contents,
                        const QString &path,
                        const QMimeType &mimeType,
                        int index,
                        int level)
{
    const auto resource = contents.resource(index, level);
    switch (resource.type()) {
    case ImageResource::Type::Image:
        extractData(resource.image(), path, mimeType);
        break;
    case ImageResource::Type::CubeTexture:
        extractData(resource.cubeTexture(), path, mimeType);
        break;
    case ImageResource::Type::VolumeTexture:
        extractData(resource.volumeTexture(), path, mimeType);
        break;
    default:
        throw RuntimeError(ExtractTool::tr("Invalid type for resource "
                                           "(index = %1, level = %2").
                           arg(index).
                           arg(level));
    }
}

static void extractData(const ImageContents &contents,
                        const QString &path,
                        const QMimeType &mimeType,
                        int index)
{
    const int count = contents.header().mipmapCount();
    if (count == 1) {
        extractData(contents, path, mimeType, index, 0);
    } else {
        for (int level = 0; level < count; ++level) {
            extractData(contents,
                        path + QStringLiteral("/mipmap %1").arg(level),
                        mimeType,
                        index,
                        level);
        }
    }
}

static void extractData(const ImageContents &contents,
                        const QString &path,
                        const QMimeType &mimeType)
{
    const int count = contents.header().imageCount();
    if (count == 1) {
        extractData(contents, path, mimeType, 0);
    } else {
        for (int index = 0; index < count; ++index) {
            extractData(contents,
                        path + QStringLiteral("/frame %1").arg(index),
                        mimeType,
                        index);
        }
    }
}

static void extract(const Options &options)
{
    ImageIO io(options.inputFile);
    if (!options.inputMimeType.isEmpty())
        io.setMimeType(options.inputMimeType);
    const auto result = io.read();
    const auto &status = result.first;
    const auto &contents = result.second;
    const auto header = contents.header();
    if (!status) {
        throw RuntimeError(ExtractTool::tr("Can't read image %1: %2").
                           arg(options.inputFile).
                           arg(status.toString()));
    }

    QFileInfo info(options.outputFolder);
    if (!info.exists()) {
        throw RuntimeError(ExtractTool::tr("%1 doesn't exist").
                           arg(info.absoluteFilePath()));
    }

    const auto outputPath = options.outputFolder
            + '/' + QFileInfo(options.inputFile).baseName();
    info = QFileInfo(outputPath);
    if (info.exists()) {
        if (options.force) {
            removePath(outputPath);
        } else {
            throw RuntimeError(ExtractTool::tr("%1 already exists").
                               arg(info.absoluteFilePath()));
        }
    }

    QMimeDatabase db;
    const auto mt = !options.outputMimeType.isEmpty()
            ? db.mimeTypeForName(options.outputMimeType)
            : QMimeDatabase().mimeTypeForFile(options.inputFile);

    QDir dir(info.absolutePath());
    dir.mkpath(info.baseName());
    extractHeader(header, info.absoluteFilePath());
    extractData(contents,
                info.absoluteFilePath() + QStringLiteral("/data"),
                mt);
}

} // namespace

ExtractTool::ExtractTool()
{

}

QByteArray ExtractTool::id() const
{
    return toolId;
}

QString ExtractTool::decription() const
{
    return ExtractTool::tr("Extract images from complex file (cube/volume map)");
}

int ExtractTool::run(const QStringList& arguments)
{
    const auto options = parseOptions(arguments);
    extract(options);
    return 0;
}

} // namespace ImageTool
