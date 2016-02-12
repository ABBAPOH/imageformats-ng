#ifndef TESTHANDLER_H
#define TESTHANDLER_H

#include <ImageIOHandler>

#include "testhandler_p.h"

class TestHandler : public ImageIOHandler
{
public:
    virtual bool canRead();
    virtual bool readHeader(ImageContents &contents);
    virtual bool read(ImageContents &contents, const ImageOptions &options);
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
    ImageIOHandler *create(QIODevice *device, const QMimeType &mimeType) Q_DECL_OVERRIDE;
    Capabilities capabilities(QIODevice *device, const QMimeType &mimeType) const Q_DECL_OVERRIDE;
};

#endif // TESTHANDLER_H
