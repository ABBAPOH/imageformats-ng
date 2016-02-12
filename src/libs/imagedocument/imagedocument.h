#ifndef IMAGEDOCUMENT_H
#define IMAGEDOCUMENT_H

#include <ImageContents>
#include <ImageOptions>

#include <QtCore/QObject>
#include <QtCore/QVariant>
#include <QtGui/QImage>

class ImageDocumentPrivate;
class IMAGEDOCUMENT_EXPORT ImageDocument : public QObject
{
    Q_OBJECT
    Q_DECLARE_PRIVATE(ImageDocument)
    Q_DISABLE_COPY(ImageDocument)

    Q_PROPERTY(ImageContents contents READ contents WRITE setContents NOTIFY contentsChanged)

public:
    explicit ImageDocument(QObject *parent = 0);
    ~ImageDocument();

    ImageContents contents() const;
    void setContents(const ImageContents &contents);

signals:
    void contentsChanged();

protected:
    QScopedPointer<ImageDocumentPrivate> d_ptr;
};

#endif // IMAGEDOCUMENT_H
