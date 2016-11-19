#pragma once

#include <QtCore/QScopedPointer>
#include <QtCore/QPoint>
#include <QtCore/QSize>
#include <QtGui/QImage>

class ImageDocument;

class ImageDocumentItemPrivate;
class ImageDocumentItem
{
public:
    explicit ImageDocumentItem(ImageDocument *document);
    ~ImageDocumentItem();

    ImageDocument *document() const;

    QPoint position() const;
    void setPosition(const QPoint &pos);

    QSize size() const;
    void setSize(const QSize &size);

    qreal rotation(Qt::Axis axis) const;
    void setRotation(Qt::Axis axis, qreal rotation);

    QImage image() const;
    void setImage(const QImage &image);

private:
    QScopedPointer<ImageDocumentItemPrivate> d;
};
