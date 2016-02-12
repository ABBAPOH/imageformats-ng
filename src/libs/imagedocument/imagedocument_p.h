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

class ImageDocumentPrivate
{
    Q_DECLARE_PUBLIC(ImageDocument)
    ImageDocument *q_ptr {nullptr};

public:
    explicit ImageDocumentPrivate(ImageDocument *qq);

    ImageContents contents;
};

#endif // IMAGEDOCUMENT_P

