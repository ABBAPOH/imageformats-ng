#pragma once

#include "imageformatsng_global.h"

#include <QtCore/QCoreApplication>
#include <QtCore/QSharedDataPointer>
#include <QtGui/QImage>

class ImageHeaderData;

class IMAGEFORMATSNG_EXPORT ImageHeader
{
    Q_GADGET
    Q_DECLARE_TR_FUNCTIONS(ImageHeader)
public:
    enum Type {
        Invalid,
        Image,
        Cubemap,
        VolumeTexture
    };
    Q_ENUMS(Type)

    ImageHeader();
    ImageHeader(const ImageHeader &);
    ImageHeader(ImageHeader &&);
    ~ImageHeader();

    ImageHeader &operator=(const ImageHeader &);
    ImageHeader &operator=(ImageHeader &&);

    bool isNull() const;

    Type type() const;
    void setType(Type t);

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

    bool validate(QString *error = nullptr);

private:
    QSharedDataPointer<ImageHeaderData> d;

    friend bool IMAGEFORMATSNG_EXPORT operator ==(const ImageHeader &lhs, const ImageHeader &rhs);
    friend bool IMAGEFORMATSNG_EXPORT operator !=(const ImageHeader &lhs, const ImageHeader &rhs);
};

bool IMAGEFORMATSNG_EXPORT operator ==(const ImageHeader &lhs, const ImageHeader &rhs);
bool IMAGEFORMATSNG_EXPORT operator !=(const ImageHeader &lhs, const ImageHeader &rhs);
