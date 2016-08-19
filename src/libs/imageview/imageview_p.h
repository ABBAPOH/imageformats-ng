#pragma once

#include "imageview.h"

class ImageControl;

class ImageViewPrivate
{
    Q_DECLARE_PUBLIC(ImageView)
    Q_DISABLE_COPY(ImageViewPrivate)

public:
    explicit ImageViewPrivate(ImageView *qq) : q_ptr(qq) {}

    void init();

    QScopedPointer<ImageControl> control;

protected:
    ImageView *q_ptr {Q_NULLPTR};
};
