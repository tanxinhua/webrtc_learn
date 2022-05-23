#pragma once

#include <QtCore/qglobal.h>

#ifndef BUILD_STATIC
# if defined(CONTENT_CAPTURE_LIB)
#  define CONTENT_CAPTURE_EXPORT Q_DECL_EXPORT
# else
#  define CONTENT_CAPTURE_EXPORT Q_DECL_IMPORT
# endif
#else
# define CONTENT_CAPTURE_EXPORT
#endif
