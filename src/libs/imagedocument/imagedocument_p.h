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
    enum OpenFlag { OpenHeader = 0x1, OpenData = 0x2 };
    Q_DECLARE_FLAGS(OpenFlags, OpenFlag)

    explicit ImageDocumentPrivate(ImageDocument *qq);

    void changed() Q_DECL_OVERRIDE { killHandler(); }

    bool initHandler();
    bool ensureHandlerInitialised() const;
    void killHandler();

    std::unique_ptr<ImageIOHandler> handler;

    QByteArray subType;
    OpenFlags openFlags;
    ReadOptions readOptions;
    WriteOptions writeOptions;

    ImageContents contents;
};

#endif // IMAGEDOCUMENT_P

