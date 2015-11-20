#ifndef QHTTPCONTEXT_H
#define QHTTPCONTEXT_H

#include "qtserverglobal.h"
#include <qhttphandler.h>

QT_BEGIN_NAMESPACE

class QHttpRequest;
class QHttpResponse;
class QHttpService;

typedef QVector<QHttpHandlerRef> QHttpHandlerSet;
typedef QVector<QHttpHandlerSet> QHttpHandlerSetChain;

class Q_SERVER_EXPORT QHttpContext
{
public:
	QHttpContext();

	void next();
	void skip();

public:
	QHttpRequest * req;
	QHttpResponse * res;
	QHttpService * service;

	const QHttpHandlerSetChain * chain;
	int i, j;
};

QT_END_NAMESPACE

#endif
