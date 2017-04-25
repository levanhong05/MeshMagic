#ifndef FLOWLAYOUT_GLOBAL
#define FLOWLAYOUT_GLOBAL

#include <QtCore/qglobal.h>

#if defined(FLOWLAYOUT_LIBRARY)
#  define FLOWLAYOUTSHARED_EXPORT Q_DECL_EXPORT
#else
#  define FLOWLAYOUTSHARED_EXPORT Q_DECL_IMPORT
#endif

#endif // FLOWLAYOUT_GLOBAL

