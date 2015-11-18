#ifndef QHTTPROUTER_H
#define QHTTPROUTER_H

#include <QtCore/QObject>
#include "qhttprequest.h"
#include "qhttpresponse.h"
#include "qhttpcontext.h"
#include "qhttphandler.h"

QT_BEGIN_NAMESPACE

#define LAMBDA_IMPLICT_CONVERTION_WORKAROUND(func)																								\
	template<typename T0 = QHttpHandlerRef, typename T1 = QHttpHandlerRef, typename T2 = QHttpHandlerRef, typename T3 = QHttpHandlerRef,		\
		     typename T4 = QHttpHandlerRef, typename T5 = QHttpHandlerRef, typename T6 = QHttpHandlerRef, typename T7 = QHttpHandlerRef>		\
	inline void func(const T0 & h0 = T0(), const T1 & h1 = T1(), const T2 & h2 = T2(), const T3 & h3 = T3(),									\
					const T4 & h4 = T4(), const T5 & h5 = T5(), const T6 & h6 = T6(), const T7 & h7 = T7())										\
	{																																			\
		func(QHttpHandlerRef(h0), QHttpHandlerRef(h1), QHttpHandlerRef(h2), QHttpHandlerRef(h3),												\
			QHttpHandlerRef(h4), QHttpHandlerRef(h5), QHttpHandlerRef(h6), QHttpHandlerRef(h7));												\
	}

class QHttpRouter : public QObject
{
	Q_OBJECT
public:
	QHttpRouter(QObject * parent = 0);

public:
	void use(QHttpHandlerRef h0 = QHttpHandlerRef(), QHttpHandlerRef h1 = QHttpHandlerRef(),
			 QHttpHandlerRef h2 = QHttpHandlerRef(), QHttpHandlerRef h3 = QHttpHandlerRef(), 
			 QHttpHandlerRef h4 = QHttpHandlerRef(), QHttpHandlerRef h5 = QHttpHandlerRef(),
			 QHttpHandlerRef h6 = QHttpHandlerRef(), QHttpHandlerRef h7 = QHttpHandlerRef()); 

	void get(QHttpHandlerRef h0 = QHttpHandlerRef(), QHttpHandlerRef h1 = QHttpHandlerRef(),
			 QHttpHandlerRef h2 = QHttpHandlerRef(), QHttpHandlerRef h3 = QHttpHandlerRef(), 
			 QHttpHandlerRef h4 = QHttpHandlerRef(), QHttpHandlerRef h5 = QHttpHandlerRef(),
			 QHttpHandlerRef h6 = QHttpHandlerRef(), QHttpHandlerRef h7 = QHttpHandlerRef()); 

	LAMBDA_IMPLICT_CONVERTION_WORKAROUND(use);
	LAMBDA_IMPLICT_CONVERTION_WORKAROUND(get);
public:
	virtual void invoke(QHttpRequest & req, QHttpResponse & res, QHttpContext & ctx);
};

QT_END_NAMESPACE

#endif
