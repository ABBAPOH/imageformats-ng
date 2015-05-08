#ifndef IMAGEDOCUMENT_P
#define IMAGEDOCUMENT_P

#include "imagedocument.h"

#include <QtCore/QHash>
#include <QtCore/QMap>

class ImageIOHandler;
class ImageIOHandlerPlugin;

class ImageDocumentPrivate
{
    Q_DECLARE_PUBLIC(ImageDocument)

    ImageDocument *q_ptr;

public:
    explicit ImageDocumentPrivate(ImageDocument *qq) : q_ptr(qq) {}

    void init();
    bool initHandler();
    void killHandler();

    QIODevice *device;
    QMimeType mimeType;
    QString errorString;
    int mipmapCount;
    int frameCount;
    ImageIndex::Sides sides;
//    int sliceCount;

    QMap<ImageIndex, QImage> images;

    ImageIOHandler *handler;
};

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

