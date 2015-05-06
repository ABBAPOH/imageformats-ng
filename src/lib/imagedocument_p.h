#ifndef IMAGEDOCUMENT_P
#define IMAGEDOCUMENT_P

#include "imagedocument.h"

class ImageDocumentPrivate
{
public:
    void init();

    QIODevice *device;
    QMimeType mimeType;
    QString errorString;
    int mipmapCount;
    int frameCount;
    ImageIndex::Sides sides;
    int sliceCount;
};

#endif // IMAGEDOCUMENT_P

