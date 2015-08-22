#include "imageviewitem.h"

#include <ImageDocument>

ImageViewItem::ImageViewItem()
{
    control.reset(new ImageControl);
    auto doc = new ImageDocument(control.data());
    control->setDocument(doc);
    control->document()->setFileName("/Users/arch/images/cubemap.dds");
    control->document()->open();

    connect(this, &QQuickPaintedItem::widthChanged, this, &ImageViewItem::makeDirty);
    connect(this, &QQuickPaintedItem::heightChanged, this, &ImageViewItem::makeDirty);
}

void ImageViewItem::paint(QPainter *painter)
{
    if (dirty) {
        QResizeEvent event(QSize(width(), height()), contentsSize());
        control->resizeEvent(&event);
        dirty = false;
    }
    control->paint(painter);
}

void ImageViewItem::makeDirty()
{
    dirty = true;
}

