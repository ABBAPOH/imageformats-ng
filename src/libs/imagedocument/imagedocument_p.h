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

    ImageContents contents;
};

#endif // IMAGEDOCUMENT_P

