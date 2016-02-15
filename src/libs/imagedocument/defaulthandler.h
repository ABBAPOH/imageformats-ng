#ifndef DEFAULTHANDLER_H
#define DEFAULTHANDLER_H

#include "imageiohandler.h"

class DefaultHandler : public ImageIOHandler
{
public:
    DefaultHandler();

    bool canRead() Q_DECL_OVERRIDE { return true; }
    bool read(ImageContents &contents, const ImageOptions &options) Q_DECL_OVERRIDE;
    bool write(const ImageContents &contents, const ImageOptions &options) Q_DECL_OVERRIDE;
};

class DefaultHandlerPlugin : public ImageIOHandlerPlugin
{
public:
    QByteArray name() const override;
    Capabilities capabilities(QIODevice *device, const QMimeType &mimeType) const Q_DECL_OVERRIDE;
    DefaultHandler *create(QIODevice *device, const QMimeType &mimeType) Q_DECL_OVERRIDE;
};

#endif // DEFAULTHANDLER_H
