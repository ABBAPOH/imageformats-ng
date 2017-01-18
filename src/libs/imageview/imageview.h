#pragma once

#include "imageview_global.h"

#include <ImageDocument>
#include <QtWidgets/QAbstractScrollArea>

class ImageDocument;

class ImageViewPrivate;
class IMAGEVIEW_EXPORT ImageView : public QAbstractScrollArea
{
    Q_OBJECT
    Q_DECLARE_PRIVATE(ImageView)
    Q_DISABLE_COPY(ImageView)

    Q_PROPERTY(ImageDocumentPointer document READ document WRITE setDocument NOTIFY documentChanged)
public:
    enum Action {
        ZoomIn,
        ZoomOut,
        NormalSize,
        RotateLeft,
        RotateRight,
        FlipHorizontally,
        FlipVertically,

        ActionsCount
    };
    Q_ENUMS(Position)

    explicit ImageView(QWidget *parent = Q_NULLPTR);
    ~ImageView();

    QAction *action(Action action) const;

    ImageDocumentPointer document() const;
    void setDocument(const ImageDocumentPointer &doc);

public slots:
    void jumpTo(int index, int level);

    void zoomIn();
    void zoomOut();
    void normalSize();
    void rotateLeft();
    void rotateRight();
    void flipHorizontally();
    void flipVertically();

private slots:
    void onScrollBarValueChanged();
    void onPositionChanged(const QPoint &pos);
    void onPositionBoundsChanged();

signals:
    void documentChanged();

protected:
    void paintEvent(QPaintEvent *event) override;
    void mousePressEvent(QMouseEvent *event) override;
    void mouseMoveEvent(QMouseEvent *event) override;
    void mouseReleaseEvent(QMouseEvent *event) override;
    bool viewportEvent(QEvent *event) override;

protected:
    QScopedPointer<ImageViewPrivate> d_ptr;
};
