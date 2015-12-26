#ifndef QHTTPROUTER_H
#define QHTTPROUTER_H

#include "qtserverglobal.h"
#include <QtCore/QObject>
#include <QtCore/QVector>
#include "qhttprequest.h"
#include "qhttpresponse.h"
#include "qhttpcontext.h"
#include "qhttphandler.h"

QT_BEGIN_NAMESPACE

#define DECLARE_METHOD_METHOD(method)															\
	inline void method(QHttpHandlerRef h0 = QHttpHandlerRef(), QHttpHandlerRef h1 = QHttpHandlerRef(),	\
			 QHttpHandlerRef h2 = QHttpHandlerRef(), QHttpHandlerRef h3 = QHttpHandlerRef(),	\
			 QHttpHandlerRef h4 = QHttpHandlerRef(), QHttpHandlerRef h5 = QHttpHandlerRef(),	\
			 QHttpHandlerRef h6 = QHttpHandlerRef(), QHttpHandlerRef h7 = QHttpHandlerRef()) {	\
		this->handle(#method, h0, h1, h2, h3, h4, h5, h6, h7);									\
	}


class Q_SERVER_EXPORT QHttpRouter : public QObject, QHttpHandler
{
	Q_OBJECT
public:
	QHttpRouter(QObject * parent = 0);

public:
	// specific handler set
	DECLARE_METHOD_METHOD(use);

	// specific handler set
	DECLARE_METHOD_METHOD(all);

	DECLARE_METHOD_METHOD(get);
	DECLARE_METHOD_METHOD(post);
public:
	virtual int invoke(QHttpContext & ctx);

private:
	void handle(const char * method, 
				QHttpHandlerRef h0, QHttpHandlerRef h1,
				QHttpHandlerRef h2, QHttpHandlerRef h3, 
				QHttpHandlerRef h4, QHttpHandlerRef h5,
				QHttpHandlerRef h6, QHttpHandlerRef h7);

private:
	QHttpHandlerSetChain m_hsc;
};

QT_END_NAMESPACE

#endif
