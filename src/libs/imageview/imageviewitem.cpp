#include "imageviewitem.h"

#include <QDebug>
#include <QQuickWindow>
#include <ImageDocument>
#include <ImageIO>

ImageViewItem::ImageViewItem()
{
    control.reset(new ImageControl);


    auto doc = ImageDocumentPointer(new ImageDocument(control.data()));
    control->setDocument(doc);

    connect(this, &QQuickPaintedItem::widthChanged, this, &ImageViewItem::onSizeChanged);
    connect(this, &QQuickPaintedItem::heightChanged, this, &ImageViewItem::onSizeChanged);

    ImageIO io("/Users/arch/images/cubemap.dds");
    auto result = io.read();
    if (!result.first)
        return;
    doc->setContents(result.second);

    setAcceptedMouseButtons(Qt::LeftButton);
}

void ImageViewItem::paint(QPainter *painter)
{
    control->paint(painter);
}

void ImageViewItem::mousePressEvent(QMouseEvent *event)
{
    control->mousePressEvent(event);
}

void ImageViewItem::mouseMoveEvent(QMouseEvent *event)
{
    control->mouseMoveEvent(event);
}

void ImageViewItem::mouseReleaseEvent(QMouseEvent *event)
{
    control->mouseReleaseEvent(event);
}

void ImageViewItem::onSizeChanged()
{
    control->setSize(width(), height());
}

