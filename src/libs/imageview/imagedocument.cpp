#include "imagedocument.h"
#include "imagedocument_p.h"

ImageDocumentPrivate::ImageDocumentPrivate(ImageDocument *qq) :
    DocumentPrivate(qq)
{
}

ImageDocumentPrivate::~ImageDocumentPrivate()
{
}

void ImageDocumentPrivate::onItemChanged(ImageDocumentItem *item)
{
    Q_Q(ImageDocument);
    Q_ASSERT(item);
    emit q->itemChanged(item);
}

ImageDocument::ImageDocument(QObject *parent) :
    Document(*new ImageDocumentPrivate(this), parent)
{
}

ImageDocument::~ImageDocument()
{
}

int ImageDocument::imageCount() const
{
    Q_D(const ImageDocument);
    return d->imageCount;
}

int ImageDocument::mipmapCount() const
{
    Q_D(const ImageDocument);
    return d->mipmapCount;
}

ImageDocumentItem *ImageDocument::item(int index, int level) const
{
    Q_D(const ImageDocument);
    if (index < 0 || index >= d->imageCount)
        return nullptr;
    if (level < 0 || level >= d->mipmapCount)
        return nullptr;
    return d->items.at(ImageDocumentPrivate::ImageIndex(index, level)).get();
}

ImageContents ImageDocument::toContents() const
{
    Q_D(const ImageDocument);
    ImageHeader header;
    header.setType(d->type);
    header.setSize(d->size);
    header.setImageFormat(d->imageFormat);
    header.setName(d->name);
    header.setImageCount(d->imageCount);
    header.setHasMipmaps(d->mipmapCount > 1);
    header.setFrameDelay(d->frameDelay);
    header.setLoopCount(d->loopCount);
    ImageContents result(header);
    for (int index = 0; index < d->imageCount; ++index) {
        for (int level = 0; level < d->mipmapCount; ++level) {
            const auto item = d->items.at(ImageDocumentPrivate::ImageIndex(index, level)).get();
            const auto image = item->image();
            QTransform matrix;
            matrix.rotate(item->rotation(Qt::XAxis), Qt::XAxis);
            matrix.rotate(item->rotation(Qt::YAxis), Qt::YAxis);
            matrix.rotate(item->rotation(Qt::ZAxis), Qt::ZAxis);
            matrix = QImage::trueMatrix(matrix, image.width(), image.height());
            result.setImage(image.transformed(matrix), index, level);
        }
    }
    return result;
}

void ImageDocument::setContents(const ImageContents &contents)
{
    Q_D(ImageDocument);

    if (contents.isNull()) {
        clear();
        return;
    }

    const auto header = contents.header();
    d->type = header.type();
    d->size = header.size();
    d->imageFormat = header.imageFormat();
    d->imageCount = header.imageCount();
    d->mipmapCount = header.mipmapCount();
    d->frameDelay = header.frameDelay();
    d->loopCount = header.loopCount();
    for (int index = 0; index < d->imageCount; ++index) {
        for (int level = 0; level < d->mipmapCount; ++level) {
            auto image = contents.image(index, level);
            std::unique_ptr<ImageDocumentItem> item(new ImageDocumentItem(this));
            item->setSize(image.size());
            item->setImage(image);
            d->items[ImageDocumentPrivate::ImageIndex(index, level)] = std::move(item);
        }
    }

    emit contentsChanged();
}

void ImageDocument::clear()
{
    Q_D(ImageDocument);
    d->type = ImageHeader::Type::Invalid;
    d->size = QSize();
    d->imageFormat = QImage::Format::Format_Invalid;
    d->name.clear();
    d->imageCount = 0;
    d->mipmapCount = 0;
    d->frameDelay = 0;
    d->loopCount = -1;
    d->exifMeta.clear();
    d->items.clear();
    emit contentsChanged();
}

ImageDocumentPrivate *ImageDocument::documentHandle() const
{
    return static_cast<ImageDocumentPrivate *>(d_ptr.data());
}
