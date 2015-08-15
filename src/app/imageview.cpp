#include "imageview.h"
#include "imageview_p.h"

#include <ImageControl>

#include <QtGui/QPainter>

void ImageViewPrivate::init()
{
    Q_Q(ImageView);

    control.reset(new ImageControl(q));
    QObject::connect(control.data(), &ImageControl::documentChanged, q, &ImageView::documentChanged);
}

ImageView::ImageView(QWidget *parent) :
    QAbstractScrollArea(parent),
    d_ptr(new ImageViewPrivate(this))
{
    Q_D(ImageView);
    d->init();
}

ImageView::~ImageView()
{
}

ImageDocument *ImageView::document() const
{
    Q_D(const ImageView);
    return d->control->document();
}

void ImageView::setDocument(ImageDocument *doc)
{
    Q_D(ImageView);
    d->control->setDocument(doc);
}

void ImageView::paintEvent(QPaintEvent *event)
{
    Q_UNUSED(event);
    Q_D(ImageView);
    QPainter painter(viewport());
    d->control->paint(&painter);
}
