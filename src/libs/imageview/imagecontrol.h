#ifndef IMAGECONTROL_H
#define IMAGECONTROL_H

#include <QtCore/QObject>
#include <QtCore/QSize>

class QPainter;
class QResizeEvent;

class ImageDocument;

class ImageControlPrivate;
class IMAGEVIEW_EXPORT ImageControl : public QObject
{
    Q_OBJECT
    Q_DECLARE_PRIVATE(ImageControl)

    Q_PROPERTY(ImageDocument * document READ document WRITE setDocument NOTIFY documentChanged)
    Q_PROPERTY(QSize size READ size WRITE setSize NOTIFY sizeChanged)
public:
    explicit ImageControl(QObject *parent = 0);
    ~ImageControl();

    ImageDocument *document() const;
    void setDocument(ImageDocument *doc);

    QSize size() const;
    void setSize(const QSize &size);
    void setSize(int width, int height);

    void paint(QPainter *painter);

public slots:
    void jumpTo(int index, int level);
    void zoomIn();
    void zoomOut();

signals:
    void documentChanged();
    void sizeChanged(const QSize &size);
    void updateRequested();

protected:
    QScopedPointer<ImageControlPrivate> d_ptr;
};

#endif // IMAGECONTROL_H
