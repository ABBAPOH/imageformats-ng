#include "imageitem.h"

#include <ImageDocument>

ImageItem::ImageItem()
{
    control.reset(new ImageControl);
    auto doc = new ImageDocument(control.data());
    control->setDocument(doc);
    control->document()->setFileName("/Users/arch/images/cubemap.dds");
    control->document()->open();

    connect(this, &QQuickPaintedItem::widthChanged, this, &ImageItem::onContentsSizeChanged);
    connect(this, &QQuickPaintedItem::heightChanged, this, &ImageItem::onContentsSizeChanged);
}

void ImageItem::paint(QPainter *painter)
{
    if (dirty) {
        QResizeEvent event(QSize(width(), height()), contentsSize());
        control->resizeEvent(&event);
        dirty = false;
    }
    control->paint(painter);
}

void ImageItem::onContentsSizeChanged()
{
    dirty = true;
    QResizeEvent event(contentsSize(), contentsSize());
//    event.size() = ;
    control->resizeEvent(&event);
}

