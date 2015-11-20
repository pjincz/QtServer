#ifndef QHTTPHANDLER_H
#define QHTTPHANDLER_H

#include "qtserverglobal.h"
#include <QSharedPointer>

QT_BEGIN_NAMESPACE

class QHttpContext;
class QHttpRequest;
class QHttpResponse;

typedef void FUNCTION_HANDLER_SIGN1(QHttpRequest & req, QHttpResponse & res);

class Q_SERVER_EXPORT QHttpHandler
{
public:
	enum {
		NOT_A_FILTER = -1,
		FILTER_THROUGH = 0,
		FILTER_FILTERED = 1,
	};
public:
	virtual ~QHttpHandler();

public:
	virtual int filter(QHttpContext & ctx);
	virtual void invoke(QHttpContext & ctx);
};

class Q_SERVER_EXPORT QHttpHandlerRef : public QSharedPointer<QHttpHandler>
{
public:
	QHttpHandlerRef();
	QHttpHandlerRef(QHttpHandler * h);
	QHttpHandlerRef(const char * path);
	QHttpHandlerRef(FUNCTION_HANDLER_SIGN1 func);

public:
	inline int filter(QHttpContext & ctx)
	{
		return data()->filter(ctx);
	}
	inline void invoke(QHttpContext & ctx)
	{
		data()->invoke(ctx);
	}
};

class Q_SERVER_EXPORT QHttpHandler_UrlFilter : public QHttpHandler
{
public:
	QHttpHandler_UrlFilter(const char * exp);

public:
	virtual int filter(QHttpContext & ctx);

private:
	QString m_urlexp;
};

class Q_SERVER_EXPORT QHttpHandler_MethodFilter : public QHttpHandler
{
public:
	QHttpHandler_MethodFilter(const char * method);

public:
	virtual int filter(QHttpContext & ctx);

private:
	QString m_method;
};

class Q_SERVER_EXPORT QHttpHandler_Method1 : public QHttpHandler
{
public:
	QHttpHandler_Method1(FUNCTION_HANDLER_SIGN1 func);

public:
	virtual void invoke(QHttpContext & ctx);

private:
	FUNCTION_HANDLER_SIGN1 * m_func;
};

QT_END_NAMESPACE

#endif