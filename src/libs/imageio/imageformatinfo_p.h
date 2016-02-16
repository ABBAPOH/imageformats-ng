#ifndef IMAGEFORMATINFO_P_H
#define IMAGEFORMATINFO_P_H

#include "imageformatinfo.h"

class ImageFormatInfoData : public QSharedData
{
public:
    QByteArray name;
    QMimeType mimeType;
    ImageFormatInfo::Capabilities capabilities;
    QVector<QByteArray> subTypes;
    QMap<QByteArray, QVector<ImageOptions::Option>> options;
};

#endif // IMAGEFORMATINFO_P_H
