#ifndef IMAGEDOCUMENT_P
#define IMAGEDOCUMENT_P

#include "imagedocument.h"

class ImageDocumentPrivate
{
public:
    void init();

    QIODevice *device;
    QByteArray format;
    QString errorString;
};

#endif // IMAGEDOCUMENT_P

