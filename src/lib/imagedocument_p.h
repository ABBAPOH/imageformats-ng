#ifndef IMAGEDOCUMENT_P
#define IMAGEDOCUMENT_P

#include "abstractdocument_p.h"
#include "imagedocument.h"
#include "imageiohandler.h"

#include <QtCore/QFile>
#include <QtCore/QHash>
#include <QtCore/QMap>

class ImageIOHandler;
class ImageIOHandlerPlugin;

class ImageDocumentPrivate : public AbstractDocumentPrivate
{
    Q_DECLARE_PUBLIC(ImageDocument)

public:
    explicit ImageDocumentPrivate(ImageDocument *qq);

    void changed() Q_DECL_OVERRIDE { killHandler(); }

    bool initHandler();
    bool ensureHandlerInitialised() const;
    bool ensureDeviceOpened(QIODevice::OpenMode mode);
    void killHandler();

    static QString errorString(ImageError::ErrorCode code);

    ImageIOHandler *handler;
    ImageError error;

    ImageContents contents;
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

