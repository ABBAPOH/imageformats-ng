#ifndef IMAGEDOCUMENT_H
#define IMAGEDOCUMENT_H

#include "imagecontents.h"
#include "imageerror.h"
#include "readoptions.h"
#include "writeoptions.h"

#include <QtCore/QMimeType>
#include <QtCore/QObject>
#include <QtCore/QVariant>
#include <QtGui/QImage>

class ImageDocumentPrivate;
class ImageDocument : public QObject
{
    Q_OBJECT
    Q_DECLARE_PRIVATE(ImageDocument)
    Q_DISABLE_COPY(ImageDocument)

public:
    explicit ImageDocument(QObject *parent = 0);
    ~ImageDocument();

    ImageContents contents() const;
    void setContents(const ImageContents &contents);

    bool hasError() const;
    ImageError error() const;

    bool read(QIODevice *device, const ReadOptions &options = ReadOptions());
    bool read(const QString &fileName, const ReadOptions &options = ReadOptions());

    bool write(QIODevice *device, const WriteOptions &options = WriteOptions());
    bool write(const QString &fileName, const WriteOptions &options = WriteOptions());

protected:
    ImageDocumentPrivate *d_ptr;
};

#endif // IMAGEDOCUMENT_H
