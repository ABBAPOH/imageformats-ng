#ifndef IMAGECONTROL_P
#define IMAGECONTROL_P

#include "imagecontrol.h"

class ImageControlPrivate
{
    Q_DECLARE_PUBLIC(ImageControl)
    Q_DISABLE_COPY(ImageControlPrivate)
public:
    explicit ImageControlPrivate(ImageControl *qq) : q_ptr(qq) {}

    void init();

    ImageDocument *doc {Q_NULLPTR};

protected:
    ImageControl *q_ptr {Q_NULLPTR};
};

#endif // IMAGECONTROL_P

