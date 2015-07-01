#include "defaulthandler.h"

#include <QImageReader>
#include <QImageWriter>

static QByteArray mimeTypeToFormat(const QString &name)
{
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
    if (reader.supportsOption(QImageIOHandler::Animation)) {
        document()->setImageCount(count);
        for (int i = 0; i < count; i++) {
            QImage image;
            const bool ok = reader.read(&image);
            if (!ok)
                return false;

            document()->setImage(image, i);
        }
    } else if (count > 0) {
        for (int i = 0; i < count; i++) {
            QImage image;
            reader.jumpToImage(i);
            const bool ok = reader.read(&image);
            if (!ok)
                return false;

            document()->setImage(image, 0, i);
        }
    } else {
        QImage image;
        const bool ok = reader.read(&image);
        if (!ok)
            return false;

        document()->setImage(image);
    }

    return true;
}

bool DefaultHandler::write()
{
    QImageWriter writer(device(), mimeTypeToFormat(mimeType()));
    const bool ok = writer.write(document()->image());
    if (!ok)
        return false;

    return true;
}

ImageIOHandlerPlugin::Capabilities DefaultHandlerPlugin::capabilities() const
{
    return Capabilities(CanRead | CanWrite);
}
