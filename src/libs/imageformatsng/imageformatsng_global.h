#pragma once

#include <QtCore/qglobal.h>

#if defined(IMAGEFORMATSNG_LIBRARY)
#  define IMAGEFORMATSNG_EXPORT Q_DECL_EXPORT
#else
#  define IMAGEFORMATSNG_EXPORT Q_DECL_IMPORT
#endif
