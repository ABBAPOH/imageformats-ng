#ifndef IMAGEDOCUMENT_P
#define IMAGEDOCUMENT_P

#include "imagedocument.h"
#include "imageiohandler.h"

#include <QtCore/QFile>
#include <QtCore/QHash>
#include <QtCore/QMap>

class ImageIOHandler;
class ImageIOHandlerPlugin;

class ImageDocumentPrivate
{
    Q_DECLARE_PUBLIC(ImageDocument)

    ImageDocument *q_ptr;

public:
    typedef QPair<int, int> ImageIndex;

    explicit ImageDocumentPrivate(ImageDocument *qq);

    void updateCaps();

    static QString errorString(ImageError::ErrorCode code);

    ImageError error;

    ImageDocument::Type type;
    int imageCount;
    int mipmapCount;
    QMap<ImageIndex, QImage> images;
    int imageDelay;
    int loopCount;
    ImageExifMeta exif;

    ImageIOHandlerPlugin::Capabilities caps;

    ImageIOHandler *handler;
};

class ImageIOHandlerDatabase
{
public:
    ImageIOHandlerDatabase();
    ~ImageIOHandlerDatabase();

    ImageIOHandler *create(const QString &mimeType);
    QStringList availableMimeTypes(ImageIOHandlerPlugin::Capabilities caps) const;
    ImageIOHandlerPlugin *plugin(const QString &mimeType) const;

    static ImageIOHandlerDatabase *instance();

private:

    QHash<QString, ImageIOHandlerPlugin *> map;
};

#endif // IMAGEDOCUMENT_P

