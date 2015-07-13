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

    Q_PROPERTY(QByteArray subType READ subType WRITE setSubType NOTIFY subTypeChanged)

public:
    enum OpenFlag { ReadHeader = 0x1, ReadData = 0x2, ReadAll = 0x3 };
    Q_DECLARE_FLAGS(OpenFlags, OpenFlag)

    explicit ImageDocument(QObject *parent = 0);
    ~ImageDocument();

    bool open() Q_DECL_OVERRIDE;
    bool open(OpenFlags flags);
    bool save() Q_DECL_OVERRIDE;

    bool hasError() const;
    ImageError error() const;

    QByteArray subType() const;
    void setSubType(QByteArray subType);

    ImageContents contents() const;
    void setContents(const ImageContents &contents);

signals:
    void subTypeChanged(QByteArray subType);
};

#endif // IMAGEDOCUMENT_H
