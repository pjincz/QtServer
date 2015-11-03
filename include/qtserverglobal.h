#ifndef QTSERVERGLOBAL_H
#define QTSERVERGLOBAL_H

#include <QtCore/qglobal.h>

QT_BEGIN_NAMESPACE

#ifndef QT_STATIC
#  if defined(QT_BUILD_SERVER_LIB)
#    define Q_SERVER_EXPORT Q_DECL_EXPORT
#  else
#    define Q_SERVER_EXPORT Q_DECL_IMPORT
#  endif
#else
#  define Q_SERVER_EXPORT
#endif

QT_END_NAMESPACE

#endif // QTSERVERGLOBAL_H
