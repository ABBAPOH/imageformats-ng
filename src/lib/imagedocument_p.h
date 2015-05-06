#ifndef IMAGEDOCUMENT_P
#define IMAGEDOCUMENT_P

#include "imagedocument.h"

#include <QtCore/QHash>

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

class ImageIOHandler;
class ImageIOHandlerPlugin;
class ImageIOHandlerDatabase
{
public:
    ImageIOHandlerDatabase();
    ~ImageIOHandlerDatabase();

    ImageIOHandler *create(const QMimeType &mimeType);

    static ImageIOHandlerDatabase *instance();

private:

    QHash<QString, ImageIOHandlerPlugin *> map;
};

#endif // IMAGEDOCUMENT_P

