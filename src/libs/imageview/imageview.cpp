#include "imageview.h"
#include "imageview_p.h"

#include <ImageControl>

#include <QtWidgets/QScrollBar>

#include <QtGui/QPainter>
#include <QtGui/QResizeEvent>

#include <QDebug>

void ImageViewPrivate::init()
{
    Q_Q(ImageView);

    q->horizontalScrollBar()->setRange(0, 0);
    q->horizontalScrollBar()->setValue(0);
    q->horizontalScrollBar()->setSingleStep(20);

    q->verticalScrollBar()->setRange(0, 0);
    q->verticalScrollBar()->setValue(0);
    q->verticalScrollBar()->setSingleStep(20);

    control.reset(new ImageControl(q));
    QObject::connect(control.data(), &ImageControl::documentChanged, q, &ImageView::documentChanged);
    QObject::connect(control.data(), &ImageControl::updateRequested,
                     q->viewport(), static_cast<void(QWidget::*)()>(&QWidget::update));

    QObject::connect(q->horizontalScrollBar(), &QAbstractSlider::valueChanged,
                     q, &ImageView::onScrollBarValueChanged);
    QObject::connect(q->verticalScrollBar(), &QAbstractSlider::valueChanged,
                     q, &ImageView::onScrollBarValueChanged);

    QObject::connect(control.data(), &ImageControl::viewportSizeChanged,
                     q, &ImageView::onViewPortSizeChanged);
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

void ImageView::onScrollBarValueChanged()
{
    Q_D(ImageView);
    d->control->setPos(QPoint(horizontalScrollBar()->value(), verticalScrollBar()->value()));
}

void ImageView::onViewPortSizeChanged(const QSize size)
{
    horizontalScrollBar()->setMinimum(-size.width());
    horizontalScrollBar()->setMaximum(size.width());

    verticalScrollBar()->setMinimum(-size.height());
    verticalScrollBar()->setMaximum(size.height());
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
