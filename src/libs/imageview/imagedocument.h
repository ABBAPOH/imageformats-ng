#pragma once

#include "imageview_global.h"

#include <Document>
#include <ImageContents>
#include <ImageOptions>

#include <QtCore/QObject>
#include <QtCore/QVariant>
#include <QtGui/QImage>

class ImageDocumentPrivate;
class IMAGEVIEW_EXPORT ImageDocument : public Document
{
    Q_OBJECT
    Q_DECLARE_PRIVATE(ImageDocument)
    Q_DISABLE_COPY(ImageDocument)

    Q_PROPERTY(ImageContents contents READ contents WRITE setContents NOTIFY contentsChanged)

public:
    explicit ImageDocument(QObject *parent = 0);
    ~ImageDocument();

    ImageContents contents() const;
    void setContents(const ImageContents &contents);

signals:
    void contentsChanged();
};
