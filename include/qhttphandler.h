#ifndef QHTTPHANDER_H
#define QHTTPHANDER_H

#include <QSharedPointer>
#include "qhttprequest.h"
#include "qhttpresponse.h"
#include "qhttpcontext.h"

#include <functional>

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
	QHttpHandlerRef(FUNCTION_HANDLER_SIGN1);
	//QHttpHandlerRef(std::function<FUNCTION_HANDLER_SIGN1> lambda);

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

QT_END_NAMESPACE

#endif
