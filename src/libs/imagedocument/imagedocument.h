#ifndef IMAGEDOCUMENT_H
#define IMAGEDOCUMENT_H

#include <AbstractDocument>
#include <ImageContents>
#include <ImageOptions>

#include <QtCore/QObject>
#include <QtCore/QVariant>
#include <QtGui/QImage>

class ImageDocumentPrivate;
class IMAGEDOCUMENT_EXPORT ImageDocument : public AbstractDocument
{
    Q_OBJECT
    Q_DECLARE_PRIVATE(ImageDocument)
    Q_DISABLE_COPY(ImageDocument)

    Q_PROPERTY(QByteArray subType READ subType WRITE setSubType NOTIFY subTypeChanged)

public:
    explicit ImageDocument(QObject *parent = 0);
    explicit ImageDocument(const QString &fileName, QObject *parent = 0);
    ~ImageDocument();

    Result openHeader();
    Result open(const ImageOptions &options = ImageOptions());
    Result save(const ImageOptions &options = ImageOptions());

    QVector<QMimeType> supportedInputMimetypes() const Q_DECL_OVERRIDE;
    QVector<QMimeType> supportedOutputMimetypes() const Q_DECL_OVERRIDE;

    QByteArray subType() const;
    void setSubType(QByteArray subType);
    QVector<QByteArray> supportedSubTypes() const;

    ImageContents contents() const;
    void setContents(const ImageContents &contents);

protected:
    bool read() Q_DECL_OVERRIDE;
    bool write() Q_DECL_OVERRIDE;

signals:
    void subTypeChanged(QByteArray subType);
};

#endif // IMAGEDOCUMENT_H
