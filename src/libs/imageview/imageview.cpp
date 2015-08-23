#include "imageview.h"
#include "imageview_p.h"

#include <ImageControl>

#include <QtGui/QPainter>
#include <QtGui/QResizeEvent>

void ImageViewPrivate::init()
{
    Q_Q(ImageView);

    control.reset(new ImageControl(q));
    QObject::connect(control.data(), &ImageControl::documentChanged, q, &ImageView::documentChanged);
    QObject::connect(control.data(), &ImageControl::updateRequested,
                     q->viewport(), static_cast<void(QWidget::*)()>(&QWidget::update));
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

void ImageView::jumpTo(int index, int level)
{
    Q_D(ImageView);
    d->control->jumpTo(index, level);
}

void ImageView::zoomIn()
{
    Q_D(ImageView);
    d->control->zoomIn();
}

void ImageView::zoomOut()
{
    Q_D(ImageView);
    d->control->zoomOut();
}

void ImageView::paintEvent(QPaintEvent *event)
{
    Q_UNUSED(event);
    Q_D(ImageView);
    QPainter painter(viewport());
    d->control->paint(&painter);
}

bool ImageView::viewportEvent(QEvent *event)
{
    switch (event->type()) {
    case QEvent::Resize : {
        Q_D(ImageView);
        d->control->setSize(static_cast<QResizeEvent *>(event)->size());
    }
    default:
        break;
    }

    return QAbstractScrollArea::viewportEvent(event);
}
