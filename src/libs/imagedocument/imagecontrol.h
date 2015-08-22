#ifndef IMAGECONTROL_H
#define IMAGECONTROL_H

#include <QtCore/QObject>

class QPainter;
class QResizeEvent;

class ImageDocument;

class ImageControlPrivate;
class IMAGEDOCUMENT_EXPORT ImageControl : public QObject
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
    void resizeEvent(QResizeEvent *event);

public slots:
    void jumpTo(int index, int level);

signals:
    void documentChanged();
    void updateRequested();

protected:
    QScopedPointer<ImageControlPrivate> d_ptr;
};

#endif // IMAGECONTROL_H
