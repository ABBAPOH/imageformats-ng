#ifndef IMAGECONTROL_H
#define IMAGECONTROL_H

#include <QtCore/QObject>

class QPainter;

class ImageDocument;

class ImageControlPrivate;
class ImageControl : public QObject
{
    Q_OBJECT
    Q_DECLARE_PRIVATE(ImageControl)

    Q_PROPERTY(ImageDocument * document READ document WRITE setDocument NOTIFY documentChanged)
public:
    explicit ImageControl(QObject *parent = 0);
    ~ImageControl();

    ImageDocument *document() const;
    void setDocument(ImageDocument *doc);

    void paint(QPainter *painter);

signals:
    void documentChanged();

protected:
    QScopedPointer<ImageControlPrivate> d_ptr;
};

#endif // IMAGECONTROL_H
