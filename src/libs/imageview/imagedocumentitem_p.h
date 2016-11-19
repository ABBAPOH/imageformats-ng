#pragma once

#include "imagedocumentitem.h"

class ImageDocumentItemPrivate
{
public:
    void emitChanged(ImageDocumentItem *q);

public:
    ImageDocument *document { nullptr };
    QPoint position;
    QSize size;
    qreal rotation[3] = { 0, 0, 0 };
    QImage image;
};
