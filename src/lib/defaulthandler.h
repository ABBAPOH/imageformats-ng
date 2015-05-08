#ifndef DEFAULTHANDLER_H
#define DEFAULTHANDLER_H

#include "imageiohandler.h"

class DefaultHandler : public ImageIOHandler
{
public:
    DefaultHandler();

    bool open(ImageDocument::OpenMode mode) Q_DECL_OVERRIDE;
    bool read() Q_DECL_OVERRIDE;
    bool write() Q_DECL_OVERRIDE;
};

class DefaultHandlerPlugin : public ImageIOHandlerPlugin
{
public:
    DefaultHandler *create() { return new DefaultHandler(); }
};

#endif // DEFAULTHANDLER_H
