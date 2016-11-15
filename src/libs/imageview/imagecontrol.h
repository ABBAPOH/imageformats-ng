#pragma once

#include "imageview_global.h"

#include <QtCore/QObject>
#include <QtCore/QPoint>
#include <QtCore/QRect>
#include <QtCore/QSize>

class QPainter;
class QResizeEvent;
class QMouseEvent;

class ImageDocument;

class ImageControlPrivate;
class IMAGEVIEW_EXPORT ImageControl : public QObject
{
    Q_OBJECT
    Q_DECLARE_PRIVATE(ImageControl)

    Q_PROPERTY(ImageDocument * document READ document WRITE setDocument NOTIFY documentChanged)
    Q_PROPERTY(QSize size READ size WRITE setSize NOTIFY sizeChanged)
    Q_PROPERTY(QPoint position READ position WRITE setPosition NOTIFY positionChanged)
    Q_PROPERTY(QRect scrollBarRanges READ scrollBarRanges NOTIFY scrollBarRangesChanged)

public:
    explicit ImageControl(QObject *parent = 0);
    ~ImageControl();

    ImageDocument *document() const;
    void setDocument(ImageDocument *doc);

    QSize size() const;
    void setSize(const QSize &size);
    void setSize(int width, int height);

    QPoint position() const;
    void setPosition(QPoint pos);

    QRect scrollBarRanges() const;

    void paint(QPainter *painter);
    void mousePressEvent(QMouseEvent *event);
    void mouseMoveEvent(QMouseEvent *event);
    void mouseReleaseEvent(QMouseEvent *event);

public slots:
    void jumpTo(int index, int level);
    void zoomIn();
    void zoomOut();
    void normalSize();

signals:
    void documentChanged();
    void sizeChanged(const QSize &size);
    void positionChanged(const QPoint &pos);
    void scrollBarRangesChanged(const QRect &rect);
    void updateRequested();

private slots:
    void onContentsChanged();

protected:
    QScopedPointer<ImageControlPrivate> d_ptr;
};

// -1. ViewPort.size = fixed, image.size = image.size * scaleFactor
// 0. Image.rect = (-w/2, -h/2, w, h)
// 1. Image.size <= ViewPort.size - pos = any
// 1. ScrollBar.MaximumValue = 0
// 2. Image.size > ViewPort.size - ViewPort.rect = ()
// 2. ScrollBar.MaximumValue = +(Image.size - ViewPort.size) / 2
// 2. ScrollBar.MinimumValue = -(Image.size - ViewPort.size) / 2
// 2. ScrollBar.Value = MinimumValue..MaximumValue
// 2. ScrollBar.Value = Viewport.pos
