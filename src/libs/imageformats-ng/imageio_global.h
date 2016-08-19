#pragma once

#include <QtCore/qglobal.h>

#if defined(IMAGEIO_LIBRARY)
#  define IMAGEIO_EXPORT Q_DECL_EXPORT
#else
#  define IMAGEIO_EXPORT Q_DECL_IMPORT
#endif
