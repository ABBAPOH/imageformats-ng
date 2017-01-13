#pragma once

#include "imageiohandler.h"

#include <QImageReader>
#include <memory>

class DefaultHandler : public ImageIOHandler
{
public:
    DefaultHandler();

public: // ImageIOHandler interface
    bool canRead() const override { return true; }
    bool readHeader(ImageHeader &header) override;
    bool read(ImageContents &contents, const ImageOptions &options) override;
    bool write(const ImageContents &contents, const ImageOptions &options) override;

private:
    std::unique_ptr<QImageReader> _reader;
};

class DefaultHandlerPlugin : public ImageIOHandlerPlugin
{
public:
    QByteArray name() const override;
    ImageFormatInfo::Capabilities capabilities(QIODevice *device, const QMimeType &mimeType) const override;
    DefaultHandler *create(QIODevice *device, const QMimeType &mimeType) override;
};

