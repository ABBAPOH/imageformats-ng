#ifndef DEFAULTHANDLER_H
#define DEFAULTHANDLER_H

#include "imageiohandler.h"

class DefaultHandler : public ImageIOHandler
{
public:
    DefaultHandler();

    bool canRead() override { return true; }
    bool read(ImageContents &contents, const ImageOptions &options) override;
    bool write(const ImageContents &contents, const ImageOptions &options) override;
};

class DefaultHandlerPlugin : public ImageIOHandlerPlugin
{
public:
    QByteArray name() const override;
    ImageFormatInfo::Capabilities capabilities(QIODevice *device, const QMimeType &mimeType) const override;
    DefaultHandler *create(QIODevice *device, const QMimeType &mimeType) override;
};

#endif // DEFAULTHANDLER_H
