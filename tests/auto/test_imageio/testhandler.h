#pragma once

#include <ImageIOHandler>

#include "testhandler_p.h"

class TestHandler : public ImageIOHandler
{
public:
    virtual bool canRead();
    virtual bool read(ImageContents &contents);
    virtual bool write(const ImageContents &contents, const ImageOptions &options);

private:
    TestImageData _data;
};

class TestHandlerPlugin : public ImageIOHandlerPlugin
{
    Q_OBJECT
    Q_DISABLE_COPY(TestHandlerPlugin)
    Q_PLUGIN_METADATA(IID "org.arch.ImageDocument.TestHandlerPlugin" FILE "test.json")

public:
    TestHandlerPlugin();
    QByteArray name() const override;
    ImageIOHandler *create(QIODevice *device, const QMimeType &mimeType) override;
    ImageFormatInfo::Capabilities capabilities(QIODevice *device, const QMimeType &mimeType) const override;
};
