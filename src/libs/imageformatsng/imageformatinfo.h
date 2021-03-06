#pragma once

#include "imageformatsng_global.h"

#include <ImageOptions>

#include <QtCore/QMimeType>
#include <QtCore/QVector>
#include <QtCore/QSharedDataPointer>

class ImageFormatInfoData;
class IMAGEFORMATSNG_EXPORT ImageFormatInfo
{
    Q_GADGET
public:
    enum Capability {
        CanRead = 0x1,
        CanWrite = 0x2,
        ReadWrite = CanRead | CanWrite
    };
    using Capabilities = QFlags<Capability>;
    Q_FLAG(Capabilities)

    ImageFormatInfo();
    ImageFormatInfo(const ImageFormatInfo &other);
    ImageFormatInfo(ImageFormatInfo &&other);
    ~ImageFormatInfo();

    ImageFormatInfo &operator=(const ImageFormatInfo &);
    ImageFormatInfo &operator=(ImageFormatInfo &&);

    QByteArray name() const;
    QMimeType mimeType() const;

    Capabilities capabilities() const;
    QString capabilitiesString() const;

    QVector<QByteArray> supportedSubTypes() const;

    ImageOptionsSet supportedOptions(const QByteArray &subType = QByteArray()) const;
    bool supportsOption(ImageOptions::Option option, const QByteArray &subType = QByteArray()) const;

    inline void swap(ImageFormatInfo &other) { qSwap(d, other.d); }

private:
    explicit ImageFormatInfo(ImageFormatInfoData *dd);

private:
    QSharedDataPointer<ImageFormatInfoData> d;

    friend class ImageIOHandlerDatabase;
};

inline bool operator==(const ImageFormatInfo &lhs, const ImageFormatInfo &rhs)
{
    return lhs.mimeType() == rhs.mimeType();
}

inline bool operator!=(const ImageFormatInfo &lhs, const ImageFormatInfo &rhs)
{
    return !(lhs == rhs);
}
