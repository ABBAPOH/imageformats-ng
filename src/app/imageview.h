#ifndef IMAGEVIEW_H
#define IMAGEVIEW_H

#include <QtWidgets/QAbstractScrollArea>

class ImageDocument;

class ImageViewPrivate;
class ImageView : public QAbstractScrollArea
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

signals:
    void documentChanged();

protected:
    void paintEvent(QPaintEvent *event);

protected:
    QScopedPointer<ImageViewPrivate> d_ptr;
};

#endif // IMAGEVIEW_H
