#pragma once

#include "imageview_global.h"

#include <Document>
#include <ImageContents>
#include <ImageOptions>

#include <QtCore/QObject>
#include <QtCore/QSharedPointer>
#include <QtCore/QVariant>
#include <QtGui/QImage>

class ImageDocumentItem;

class ImageDocumentPrivate;
class IMAGEVIEW_EXPORT ImageDocument : public Document
{
    Q_OBJECT
    Q_DECLARE_PRIVATE(ImageDocument)
    Q_DISABLE_COPY(ImageDocument)

    Q_PROPERTY(int imageCount READ imageCount)
    Q_PROPERTY(int mipmapCount READ mipmapCount)

public:
    explicit ImageDocument(QObject *parent = 0);
    ~ImageDocument();

    int imageCount() const;
    int mipmapCount() const;

    ImageDocumentItem *item(int index, int level) const;

    ImageContents toContents() const;
    void setContents(const ImageContents &toContents);

    void clear();

    ImageDocumentPrivate *documentHandle() const;

signals:
    void contentsChanged();
    void itemChanged(ImageDocumentItem *item);
};

using ImageDocumentPointer = QSharedPointer<ImageDocument>;
using ImageDocumentConstPointer = QSharedPointer<const ImageDocument>;
