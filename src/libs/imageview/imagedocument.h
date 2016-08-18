#ifndef IMAGEDOCUMENT_H
#define IMAGEDOCUMENT_H

#include "imageview_global.h"

#include <AbstractDocument>
#include <ImageContents>
#include <ImageOptions>

#include <QtCore/QObject>
#include <QtCore/QVariant>
#include <QtGui/QImage>

class ImageDocumentPrivate;
class IMAGEVIEW_EXPORT ImageDocument : public AbstractDocument
{
    Q_OBJECT
    Q_DECLARE_PRIVATE(ImageDocument)
    Q_DISABLE_COPY(ImageDocument)

    Q_PROPERTY(ImageContents contents READ contents WRITE setContents NOTIFY contentsChanged)

public:
    explicit ImageDocument(QObject *parent = 0);
    ~ImageDocument();

    QVector<QMimeType> openableMimetypes() const override;
    QVector<QMimeType> saveableMimetypes() const override;

    ImageContents contents() const;
    void setContents(const ImageContents &contents);

signals:
    void contentsChanged();

protected:
    void doOpen(const QUrl &url, const QVariantMap &options) override;
    void doSave(const QUrl &url, const QVariantMap &options) override;
};

#endif // IMAGEDOCUMENT_H
