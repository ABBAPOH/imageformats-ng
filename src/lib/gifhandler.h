#ifndef GIFHANDLER_H
#define GIFHANDLER_H

#include "imageiohandler.h"

class GifHandler : public ImageIOHandler
{
public:
    GifHandler();

    bool open(ImageDocument::OpenMode mode) Q_DECL_OVERRIDE;
    bool read() Q_DECL_OVERRIDE;
    bool write() Q_DECL_OVERRIDE;
};

class GifHandlerPlugin : public ImageIOHandlerPlugin
{
public:
    GifHandler *create() { return new GifHandler(); }
};


#endif // GIFHANDLER_H
