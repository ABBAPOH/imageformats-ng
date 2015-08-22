#ifndef IMAGEVIEW_H
#define IMAGEVIEW_H

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
    explicit ImageView(QWidget *parent = Q_NULLPTR);
    ~ImageView();

    ImageDocument *document() const;
    void setDocument(ImageDocument *doc);

public slots:
    void jumpTo(int index, int level);

signals:
    void documentChanged();

protected:
    void paintEvent(QPaintEvent *event);
    bool viewportEvent(QEvent *event);

protected:
    QScopedPointer<ImageViewPrivate> d_ptr;
};

#endif // IMAGEVIEW_H
