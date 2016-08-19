#pragma once

#include <ImageDocument>
#include <ImageIOHandler>
#include <private/DocumentPrivate>

#include <QtCore/QFile>
#include <QtCore/QHash>
#include <QtCore/QMap>

class ImageIOHandler;
class ImageIOHandlerPlugin;

class ImageDocumentPrivate : public DocumentPrivate
{
    Q_DECLARE_PUBLIC(ImageDocument)

public:
    explicit ImageDocumentPrivate(ImageDocument *qq);

    ImageContents contents;
};
