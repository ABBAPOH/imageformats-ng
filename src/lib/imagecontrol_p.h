#ifndef IMAGECONTROL_P
#define IMAGECONTROL_P

#include "imagecontrol.h"

#include <QtCore/QSize>

class ImageControlPrivate
{
    Q_DECLARE_PUBLIC(ImageControl)
    Q_DISABLE_COPY(ImageControlPrivate)
public:
    explicit ImageControlPrivate(ImageControl *qq) : q_ptr(qq) {}

    void init();

    ImageDocument *doc {Q_NULLPTR};
    QSize size;
    int currentIndex {0};
    int currentLevel {0};

protected:
    ImageControl *q_ptr {Q_NULLPTR};
};

#endif // IMAGECONTROL_P

