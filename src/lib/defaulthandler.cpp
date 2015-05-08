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

    return QByteArray();
}

DefaultHandler::DefaultHandler()
{

}

bool DefaultHandler::open(ImageDocument *document, ImageDocument::OpenMode mode)
{
    Q_UNUSED(document);
    Q_UNUSED(mode);
    return true;
}

bool DefaultHandler::read(ImageDocument *document)
{
    QImageReader reader(device(), mimeTypeToFormat(mimeType()));
    QImage image;
    const bool ok = reader.read(&image);
    if (!ok)
        return false;

    document->setImage(image);
    return true;
}

bool DefaultHandler::write(const ImageDocument *document)
{
    QImageWriter writer(device(), mimeTypeToFormat(mimeType()));
    const bool ok = writer.write(document->image());
    if (!ok)
        return false;

    return true;
}

