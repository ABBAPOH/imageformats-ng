#pragma once

#include <ImageDocument>
#include <ImageDocumentItem>
#include <ImageIOHandler>
#include <private/DocumentPrivate>

#include <QtCore/QFile>
#include <QtCore/QHash>
#include <QtCore/QMap>

#include <memory>
#include <map>

class ImageIOHandler;
class ImageIOHandlerPlugin;

class ImageDocumentPrivate : public DocumentPrivate
{
    Q_DECLARE_PUBLIC(ImageDocument)

public:
    explicit ImageDocumentPrivate(ImageDocument *qq);
    ~ImageDocumentPrivate();

    void onItemChanged(ImageDocumentItem *item);

    ImageResource::Type type {ImageResource::Type::Invalid};
    QSize size;
    QImage::Format imageFormat {QImage::Format::Format_Invalid};
    QString name;
    int imageCount {0};
    int mipmapCount {0};
    int frameDelay {0};
    int loopCount {-1};
    ImageExifMeta exifMeta;

    using ImageIndex = QPair<int, int>;
    std::map<ImageIndex, std::unique_ptr<ImageDocumentItem>> items;
};
