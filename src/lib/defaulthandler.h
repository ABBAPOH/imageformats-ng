#ifndef DEFAULTHANDLER_H
#define DEFAULTHANDLER_H

#include "imageiohandler.h"

class DefaultHandler : public ImageIOHandler
{
public:
    DefaultHandler();

    bool read() Q_DECL_OVERRIDE;
    bool write() Q_DECL_OVERRIDE;
};

class DefaultHandlerPlugin : public ImageIOHandlerPlugin
{
public:
    DefaultHandler *create() { return new DefaultHandler(); }
    Capabilities capabilities() const Q_DECL_OVERRIDE;
};

#endif // DEFAULTHANDLER_H
