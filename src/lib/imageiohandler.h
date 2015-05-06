#ifndef IMAGEIOHANDLER_H
#define IMAGEIOHANDLER_H

#include "imagedocument.h"

class ImageIOHandler
{
public:
    ImageIOHandler();
    virtual ~ImageIOHandler();
};

class ImageIOHandlerPlugin : public QObject
{
public:
    explicit ImageIOHandlerPlugin() {}

    virtual ImageIOHandler *create() = 0;
};

#endif // IMAGEIOHANDLER_H
