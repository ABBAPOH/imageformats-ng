#include "imageerror.h"

#include "imagedocument_p.h"

QString ImageError::errorString() const
{
    return _errorString.isEmpty() ? ImageDocumentPrivate::errorString(_error) : errorString();
}
