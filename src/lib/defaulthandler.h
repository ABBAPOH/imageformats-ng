#ifndef DEFAULTHANDLER_H
#define DEFAULTHANDLER_H

#include "imageiohandler.h"

class DefaultHandler : public ImageIOHandler
{
public:
    DefaultHandler();

    bool read(ImageDocument *document);
    bool write(const ImageDocument *document);
};

class DefaultHandlerPlugin : public ImageIOHandlerPlugin
{
public:
    DefaultHandler *create() { return new DefaultHandler(); }
};

#endif // DEFAULTHANDLER_H
