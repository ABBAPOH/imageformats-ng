#ifndef IMAGEDOCUMENT_H
#define IMAGEDOCUMENT_H

#include "abstractdocument.h"
#include "imagecontents.h"
#include "imageerror.h"
#include "readoptions.h"
#include "writeoptions.h"

#include <QtCore/QObject>
#include <QtCore/QVariant>
#include <QtGui/QImage>

class ImageDocumentPrivate;
class ImageDocument : public AbstractDocument
{
    Q_OBJECT
    Q_DECLARE_PRIVATE(ImageDocument)
    Q_DISABLE_COPY(ImageDocument)

public:
    explicit ImageDocument(QObject *parent = 0);
    ~ImageDocument();

    bool hasError() const;
    ImageError error() const;

    bool read(const ReadOptions & options = ReadOptions());
    bool write(const WriteOptions &options = WriteOptions());

    ImageContents contents() const;
    void setContents(const ImageContents &contents);
};

#endif // IMAGEDOCUMENT_H
