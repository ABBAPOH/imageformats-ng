#include "imageviewitem.h"

#include <QDebug>
#include <QQuickWindow>
#include <ImageDocument>
#include <ImageIO>

ImageViewItem::ImageViewItem()
{
    control.reset(new ImageControl);


    auto doc = new ImageDocument(control.data());
    control->setDocument(doc);

    connect(this, &QQuickPaintedItem::widthChanged, this, &ImageViewItem::onSizeChanged);
    connect(this, &QQuickPaintedItem::heightChanged, this, &ImageViewItem::onSizeChanged);

    ImageIO io("/Users/arch/images/cubemap.dds");
    auto contents = io.read();
    if (!contents)
        return;
    doc->setContents(*contents);
}

void ImageViewItem::paint(QPainter *painter)
{
    control->paint(painter);
}

void ImageViewItem::onSizeChanged()
{
    control->setSize(width(), height());
}

