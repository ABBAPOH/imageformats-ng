#pragma once

#include <QtCore/qglobal.h>

#if defined(IMAGEVIEW_LIBRARY)
#  define IMAGEVIEW_EXPORT Q_DECL_EXPORT
#else
#  define IMAGEVIEW_EXPORT Q_DECL_IMPORT
#endif
