#ifndef GIFHANDLER_H
#define GIFHANDLER_H

#include "imageiohandler.h"

class GifHandler : public ImageIOHandler
{
public:
    GifHandler();

    bool open(ImageDocument *document, ImageDocument::OpenMode mode);
    bool read(ImageDocument *document);
    bool write(const ImageDocument *document);
};

class GifHandlerPlugin : public ImageIOHandlerPlugin
{
public:
    GifHandler *create() { return new GifHandler(); }
};


#endif // GIFHANDLER_H
