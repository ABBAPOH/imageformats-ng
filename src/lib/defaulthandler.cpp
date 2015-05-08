#include "defaulthandler.h"

#include <QImageReader>
#include <QImageWriter>

static QByteArray mimeTypeToFormat(const QMimeType &mimeType)
{
    auto name = mimeType.name();
    if (name == "image/jpeg")
        return "jpg";

    if (name == "image/png")
        return "png";

    if (name == "image/gif")
        return "gif";

    return QByteArray();
}

DefaultHandler::DefaultHandler()
{

}

bool DefaultHandler::open(ImageDocument::OpenMode mode)
{
    Q_UNUSED(mode);
    return true;
}

bool DefaultHandler::read()
{
    QImageReader reader(device(), mimeTypeToFormat(mimeType()));

    int count = reader.imageCount();
    if (reader.supportsOption(QImageIOHandler::Animation)) {
        document()->setFrameCount(count);

        for (int i = 0; i < count; i++) {
            ImageIndex index;
            index.setFrame(i);
            QImage image;
            const bool ok = reader.read(&image);
            if (!ok)
                return false;

            document()->setImage(image, index);
        }
    } else if (count > 0) {
        document()->setMipmapCount(count);

        for (int i = 0; i < count; i++) {
            ImageIndex index;
            index.setMipmap(i);
            QImage image;
            const bool ok = reader.read(&image);
            if (!ok)
                return false;

            document()->setImage(image, index);
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

