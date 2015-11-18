#ifndef QHTTPCONTEXT_H
#define QHTTPCONTEXT_H

QT_BEGIN_NAMESPACE

class QHttpContext
{
public:
	QHttpContext();

	virtual void next();
	virtual void skip();
};

QT_END_NAMESPACE

#endif
