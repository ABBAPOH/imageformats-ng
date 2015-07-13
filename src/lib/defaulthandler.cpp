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

bool DefaultHandler::read()
{
    QImageReader reader(device(), mimeTypeToFormat(mimeType()));

    int count = reader.imageCount();
    ImageContents contents;
    if (reader.supportsOption(QImageIOHandler::Animation)) {
        contents.setImageCount(count);
        for (int i = 0; i < count; i++) {
            QImage image;
            const bool ok = reader.read(&image);
            if (!ok)
                return false;

            contents.setImage(image, i);
        }
    } else if (count > 0) {
        contents.setMipmapCount(count);
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

    document()->setContents(contents);

    return true;
}

bool DefaultHandler::write()
{
    QImageWriter writer(device(), mimeTypeToFormat(mimeType()));
    const bool ok = writer.write(document()->contents().image());
    if (!ok)
        return false;

    return true;
}

ImageIOHandlerPlugin::Capabilities DefaultHandlerPlugin::capabilities() const
{
    return Capabilities(CanRead | CanWrite);
}
