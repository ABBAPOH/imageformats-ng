#ifndef IMAGEFORMATINFO_H
#define IMAGEFORMATINFO_H

#include "imageio_global.h"

#include <ImageOptions>
#include <QtCore/QMimeType>
#include <QtCore/QVector>
#include <QtCore/QSharedDataPointer>

class ImageFormatInfoData;
class IMAGEIO_EXPORT ImageFormatInfo
{
public:
    ImageFormatInfo();
    ImageFormatInfo(const ImageFormatInfo &other);
    ImageFormatInfo(ImageFormatInfo &&other);
    ~ImageFormatInfo();

    ImageFormatInfo &operator=(const ImageFormatInfo &);
    ImageFormatInfo &operator=(ImageFormatInfo &&);

    QByteArray name() const;
    QMimeType mimeType() const;

    QVector<QByteArray> supportedSubTypes() const;

    QVector<ImageOptions::Option> supportedOptions(const QByteArray &subType = QByteArray()) const;
    bool supportsOption(ImageOptions::Option option) const;
    bool supportsOption(const QByteArray &subType, ImageOptions::Option option) const;

private:
    explicit ImageFormatInfo(const ImageFormatInfoData &data);

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

#endif // IMAGEFORMATINFO_H
