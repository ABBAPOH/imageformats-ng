#pragma once

#include "imageformatsng_global.h"

#include <ImageResource>
#include <QtCore/QCoreApplication>
#include <QtCore/QSharedDataPointer>
#include <QtGui/QImage>

class ImageHeaderData;

class IMAGEFORMATSNG_EXPORT ImageHeader
{
    Q_GADGET
    Q_DECLARE_TR_FUNCTIONS(ImageHeader)

    Q_PROPERTY(bool isNull READ isNull)
    Q_PROPERTY(ImageResource::Type type READ type WRITE setType)
    Q_PROPERTY(QImage::Format imageFormat READ imageFormat WRITE setImageFormat)
    Q_PROPERTY(QSize size READ size WRITE setSize)
    Q_PROPERTY(int width READ width WRITE setWidth)
    Q_PROPERTY(int height READ height WRITE setHeight)
    Q_PROPERTY(int depth READ depth WRITE setDepth)
    Q_PROPERTY(QString name READ name WRITE setName)
    Q_PROPERTY(int imageCount READ imageCount WRITE setImageCount)
    Q_PROPERTY(bool hasMipmaps READ hasMipmaps WRITE setHasMipmaps)
    Q_PROPERTY(int mipmapCount READ mipmapCount)
    Q_PROPERTY(int frameDelay READ frameDelay WRITE setFrameDelay)
    Q_PROPERTY(int loopCount READ loopCount WRITE setLoopCount)
public:
    ImageHeader();
    ImageHeader(const ImageHeader &);
    ImageHeader(ImageHeader &&);
    ~ImageHeader();

    ImageHeader &operator=(const ImageHeader &);
    ImageHeader &operator=(ImageHeader &&);

    bool isNull() const;

    ImageResource::Type type() const;
    void setType(ImageResource::Type type);

    QImage::Format imageFormat() const;
    void setImageFormat(QImage::Format format);

    QSize size() const;
    void setSize(QSize size);

    int width() const;
    void setWidth(int width);
    int height() const;
    void setHeight(int height);
    int depth() const;
    void setDepth(int depth);

    QString name() const;
    void setName(const QString &name);

    int imageCount() const;
    void setImageCount(int count);

    bool hasMipmaps() const;
    void setHasMipmaps(bool yes);
    int mipmapCount() const;

    int frameDelay() const;
    void setFrameDelay(int msecs);

    int loopCount() const;
    void setLoopCount(int count);

    inline void swap(ImageHeader &other) Q_DECL_NOEXCEPT { qSwap(d, other.d); }

    bool validate(QString *error = nullptr) const;

private:
    QSharedDataPointer<ImageHeaderData> d;

    friend bool IMAGEFORMATSNG_EXPORT operator ==(const ImageHeader &lhs, const ImageHeader &rhs);
    friend bool IMAGEFORMATSNG_EXPORT operator !=(const ImageHeader &lhs, const ImageHeader &rhs);
};

bool IMAGEFORMATSNG_EXPORT operator ==(const ImageHeader &lhs, const ImageHeader &rhs);
bool IMAGEFORMATSNG_EXPORT operator !=(const ImageHeader &lhs, const ImageHeader &rhs);
