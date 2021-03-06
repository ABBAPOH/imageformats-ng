#pragma once

#include "imagedocumentitem.h"

#include <QtCore/QSequentialAnimationGroup>
#include <QtCore/QPointer>

class ImageDocumentItemPrivate
{
public:
    ImageDocumentItemPrivate();
    void emitChanged(ImageDocumentItem *q);

public:
    ImageDocument *document { nullptr };
    QPoint position;
    QSize size;
    qreal rotation[3];
    qreal visualRotation[3];
    QImage image;
    QPointer<QSequentialAnimationGroup> animationGroup;
};
