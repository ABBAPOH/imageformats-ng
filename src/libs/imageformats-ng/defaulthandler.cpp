#include "defaulthandler.h"

#include <QImageReader>
#include <QImageWriter>

static QByteArray mimeTypeToFormat(const QMimeType &mimeType)
{
    const auto name = mimeType.name();
    if (name == "image/jpeg")
        return "jpg";

    if (name == "image/png")
        return "png";

    if (name == "image/gif")
        return "gif";

    if (name == "image/x-dds")
        return "dds";

    return QByteArray();
}

DefaultHandler::DefaultHandler()
{

}

bool DefaultHandler::read(ImageContents &contents, const ImageOptions &options)
{
    Q_UNUSED(options);
    QImageReader reader(device(), mimeTypeToFormat(mimeType()));

    int count = reader.imageCount();
    if (reader.supportsOption(QImageIOHandler::Animation)) {
        ImageHeader header;
        header.setImageCount(count);
        contents.setHeader(header);
        for (int i = 0; i < count; i++) {
            QImage image;
            const bool ok = reader.read(&image);
            if (!ok)
                return false;

            contents.setImage(image, i);
        }
    } else if (count > 0) {
        ImageHeader header;
        header.setMipmapCount(count);
        contents.setHeader(header);
        for (int i = 0; i < count; i++) {
            QImage image;
            reader.jumpToImage(i);
            const bool ok = reader.read(&image);
            if (!ok)
                return false;

            contents.setImage(image, 0, i);
        }
    } else {
        QImage image;
        const bool ok = reader.read(&image);
        if (!ok)
            return false;

        contents.setImage(image);
    }

    return true;
}

bool DefaultHandler::write(const ImageContents &contents, const ImageOptions &options)
{
    Q_UNUSED(options);
    QImageWriter writer(device(), mimeTypeToFormat(mimeType()));
    const bool ok = writer.write(contents.image());
    if (!ok)
        return false;

    return true;
}

QByteArray DefaultHandlerPlugin::name() const
{
    return "default";
}

DefaultHandler *DefaultHandlerPlugin::create(QIODevice *device, const QMimeType &mimeType)
{
    Q_UNUSED(device);
    Q_UNUSED(mimeType);
    return new DefaultHandler();
}

ImageFormatInfo::Capabilities DefaultHandlerPlugin::capabilities(QIODevice *device, const QMimeType &mimeType) const
{
    Q_UNUSED(device);
    Q_UNUSED(mimeType);
    return ImageFormatInfo::Capabilities(ImageFormatInfo::CanRead | ImageFormatInfo::CanWrite);
}
