#ifndef QHTTPHANDER_H
#define QHTTPHANDER_H

#include <QSharedPointer>
#include "qhttprequest.h"
#include "qhttpresponse.h"
#include "qhttpcontext.h"

QT_BEGIN_NAMESPACE

typedef void FUNCTION_HANDLER_SIGN1(QHttpRequest & req, QHttpResponse & res);

class QHttpHandler
{
public:
	virtual ~QHttpHandler();

public:
	virtual bool filtered();
	virtual void invoke(QHttpRequest & req, QHttpResponse & res, QHttpContext & ctx);
};

class QHttpHandlerRef : public QSharedPointer<QHttpHandler>
{
public:
	QHttpHandlerRef();
	QHttpHandlerRef(QHttpHandler * h);
	QHttpHandlerRef(const char * path);
	QHttpHandlerRef(FUNCTION_HANDLER_SIGN1 func);

public:
	inline bool filtered()
	{
		return data()->filtered();
	}
	inline void invoke(QHttpRequest & req, QHttpResponse & res, QHttpContext & ctx)
	{
		data()->invoke(req, res, ctx);
	}
};

class QHttpHandler_UrlFilter : public QHttpHandler
{
public:
	QHttpHandler_UrlFilter(const char * exp);

public:
	virtual bool filtered();
};

class QHttpHandler_MethodFilter : public QHttpHandler
{
public:
	QHttpHandler_MethodFilter(const char * method);

public:
	virtual bool filtered();
};

class QHttpHandler_Method1 : public QHttpHandler
{
public:
	QHttpHandler_Method1(FUNCTION_HANDLER_SIGN1 func);

public:
	virtual void invoke(QHttpRequest & req, QHttpResponse & res, QHttpContext & ctx);
};

QT_END_NAMESPACE

#endif
