#pragma once

#include "imageview_global.h"

#include <QtWidgets/QAbstractScrollArea>

class ImageDocument;

class ImageViewPrivate;
class IMAGEVIEW_EXPORT ImageView : public QAbstractScrollArea
{
    Q_OBJECT
    Q_DECLARE_PRIVATE(ImageView)
    Q_DISABLE_COPY(ImageView)

    Q_PROPERTY(ImageDocument * document READ document WRITE setDocument NOTIFY documentChanged)
public:
    enum Action {
        ZoomIn,
        ZoomOut,

        ActionsCount
    };
    Q_ENUMS(Position)

    explicit ImageView(QWidget *parent = Q_NULLPTR);
    ~ImageView();

    QAction *action(Action action) const;

    ImageDocument *document() const;
    void setDocument(ImageDocument *doc);

public slots:
    void jumpTo(int index, int level);
    void zoomIn();
    void zoomOut();

private slots:
    void onScrollBarValueChanged();
    void onViewPortSizeChanged(const QSize size);

signals:
    void documentChanged();

protected:
    void paintEvent(QPaintEvent *event);
    bool viewportEvent(QEvent *event);

protected:
    QScopedPointer<ImageViewPrivate> d_ptr;
};
