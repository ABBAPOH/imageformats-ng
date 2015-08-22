#include "imageviewitem.h"

#include <QDebug>
#include <QQuickWindow>
#include <ImageDocument>

ImageViewItem::ImageViewItem()
{
    control.reset(new ImageControl);
    auto doc = new ImageDocument(control.data());
    control->setDocument(doc);
    control->document()->setFileName("/Users/arch/images/cubemap.dds");
    control->document()->open();

    connect(this, &QQuickPaintedItem::widthChanged, this, &ImageViewItem::onSizeChanged);
    connect(this, &QQuickPaintedItem::heightChanged, this, &ImageViewItem::onSizeChanged);
}

void ImageViewItem::paint(QPainter *painter)
{
    control->paint(painter);
}

void ImageViewItem::onSizeChanged()
{
    control->setSize(width(), height());
}

