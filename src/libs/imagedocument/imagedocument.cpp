#include "imagedocument.h"
#include "imagedocument_p.h"

ImageDocumentPrivate::ImageDocumentPrivate(ImageDocument *qq) :
    q_ptr(qq)
{
}

ImageDocument::ImageDocument(QObject *parent) :
    QObject(parent),
    d_ptr(new ImageDocumentPrivate(this))
{
}

ImageDocument::~ImageDocument()
{
}

ImageContents ImageDocument::contents() const
{
    Q_D(const ImageDocument);
    return d->contents;
}

void ImageDocument::setContents(const ImageContents &contents)
{
    Q_D(ImageDocument);
    if (d->contents == contents)
        return;

    d->contents = contents;
    emit contentsChanged();
}
