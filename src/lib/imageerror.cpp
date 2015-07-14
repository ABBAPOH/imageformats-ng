#include "imageerror.h"

#include "imagedocument_p.h"

QString ImageError::errorString() const
{
    return ImageDocumentPrivate::errorString(_error);
}
