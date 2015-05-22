#ifndef IMAGEDOCUMENT_P
#define IMAGEDOCUMENT_P

#include "imagedocument.h"

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
    typedef QMap<ImageMeta::Option, QVariant> OptionMap;
    typedef QMap<ImageMeta::ExifOption, QVariant> ExifOptionMap;

    explicit ImageDocumentPrivate(ImageDocument *qq) : q_ptr(qq) {}

    void init();
    bool initHandler();
    bool ensureHandlerInitialised() const;
    bool ensureDeviceOpened(QIODevice::OpenMode mode);
    void killHandler();

    static QString errorString(ImageError::ErrorCode code);

    QIODevice *device;
    QString fileName;
    QScopedPointer<QFile> file;
    QMimeType mimeType;
    ImageError error;
    int mipmapCount;
    ImageResource::Sides sides;
    ImageMeta meta;

    QVector<ImageResource> resources;

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

