#ifndef QHTTPCONTEXT_H
#define QHTTPCONTEXT_H

QT_BEGIN_NAMESPACE

class QHttpRequest;
class QHttpResponse;

class QHttpContext
{
public:
	QHttpContext();

	virtual void next();
	virtual void skip();

public:
	QHttpRequest * req;
	QHttpResponse * res;
};

QT_END_NAMESPACE

#endif
