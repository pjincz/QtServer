#ifndef QHTTPCONTEXT_H
#define QHTTPCONTEXT_H

#include "qtserverglobal.h"
#include <QVector>

QT_BEGIN_NAMESPACE

class QHttpRequest;
class QHttpResponse;
class QHttpService;
class QHttpHandlerRef;
class QHttpHandlerSet;
typedef QVector<QHttpHandlerSet> QHttpHandlerSetChain;

class Q_SERVER_EXPORT QHttpContext
{
public:
	QHttpContext();
	QHttpContext(QHttpContext * parent);

	void next();
	void skip();

public:
	QHttpRequest * req;
	QHttpResponse * res;
	QHttpContext * parent;
	QHttpService * service;

	const QHttpHandlerSetChain * chain;
	int i, j;
};

QT_END_NAMESPACE

#endif
