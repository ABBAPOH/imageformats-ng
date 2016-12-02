#pragma once

#include <ImageContents>

struct TestImageData
{
    quint32 magic {0};
    QByteArray subType;
    ImageHeader::Type type {ImageHeader::Image};
    QSize size;
    QString name;
    QImage::Format imageFormat {QImage::Format_Invalid};
    int imageCount {0};
    bool hasMipmaps {false};
    QVector<QImage> images;
    int loopCount {-1};

    int compression {-1};
    int gamma {-1};
    int quality {-1};
};

QDataStream &operator <<(QDataStream &stream, const TestImageData &data);
QDataStream &operator >>(QDataStream &stream, TestImageData &data);
