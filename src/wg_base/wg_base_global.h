#pragma once

#include <QtCore/qglobal.h>

#ifndef BUILD_STATIC
# if defined(WG_BASE_LIB)
#  define WG_BASE_EXPORT Q_DECL_EXPORT
# else
#  define WG_BASE_EXPORT Q_DECL_IMPORT
# endif
#else
# define WG_BASE_EXPORT
#endif
