#ifndef IMAGEIO_GLOBAL_H
#define IMAGEIO_GLOBAL_H

#include <QtCore/qglobal.h>

#if defined(IMAGEIO_LIBRARY)
#  define IMAGEIO_EXPORT Q_DECL_EXPORT
#else
#  define IMAGEIO_EXPORT Q_DECL_IMPORT
#endif

#endif // IMAGEIO_GLOBAL_H
