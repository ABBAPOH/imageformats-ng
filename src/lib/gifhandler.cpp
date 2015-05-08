#include "gifhandler.h"

#include <QImageReader>
#include <QImageWriter>

GifHandler::GifHandler()
{

}

bool GifHandler::open(ImageDocument::OpenMode mode)
{
    if (mode & ImageDocument::Write)
        return false;

    return true;
}

bool GifHandler::read()
{
    QImageReader reader(device(), "gif");
    int count = reader.imageCount();
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
    return true;
}

bool GifHandler::write()
{
    return false;
}

