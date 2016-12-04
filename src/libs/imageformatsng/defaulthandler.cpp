#include "defaulthandler.h"

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

bool DefaultHandler::readHeader(ImageHeader &header)
{
    _reader.reset(new QImageReader(device(), mimeTypeToFormat(mimeType())));

    if (!_reader->supportsOption(QImageIOHandler::Size)
            || !_reader->supportsOption(QImageIOHandler::ImageFormat)) {
        return false;
    }

    header.setType(ImageHeader::Image);
    header.setSize(_reader->size());
    header.setImageFormat(_reader->imageFormat());
    int count = _reader->imageCount();
    if (_reader->supportsOption(QImageIOHandler::Animation)) {
        header.setImageCount(count);
    } else if (count > 0) {
        header.setHasMipmaps(true);
    }
    return true;
}

bool DefaultHandler::read(ImageContents &contents)
{
    int count = _reader->imageCount();
    if (_reader->supportsOption(QImageIOHandler::Animation)) {
        for (int i = 0; i < count; i++) {
            QImage image;
            const bool ok = _reader->read(&image);
            if (!ok)
                return false;

            contents.setImage(image, i);
        }
    } else if (count > 0) {
        if (count != contents.header().mipmapCount())
            return false;
        for (int i = 0; i < count; i++) {
            QImage image;
            _reader->jumpToImage(i);
            const bool ok = _reader->read(&image);
            if (!ok)
                return false;

            contents.setImage(image, 0, i);
        }
    } else {
        QImage image;
        const bool ok = _reader->read(&image);
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
