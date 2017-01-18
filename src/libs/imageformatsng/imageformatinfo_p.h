#pragma once

#include <SupportedImageOptions>

#include "imageformatinfo.h"

class ImageFormatInfoData : public QSharedData
{
public:
    QByteArray name;
    QMimeType mimeType;
    ImageFormatInfo::Capabilities capabilities;
    QVector<QByteArray> subTypes;
    QMap<QByteArray, SupportedImageOptions> options;
};
