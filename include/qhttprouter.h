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

#if __cplusplus >= 201103
#define LAMBDA_IMPLICT_CONVERTION_WORKAROUND(func)													\
	template<typename T0 = QHttpHandlerRef, typename T1 = QHttpHandlerRef,							\
			 typename T2 = QHttpHandlerRef, typename T3 = QHttpHandlerRef,							\
			 typename T4 = QHttpHandlerRef, typename T5 = QHttpHandlerRef,							\
			 typename T6 = QHttpHandlerRef, typename T7 = QHttpHandlerRef>							\
	inline void func(T0 && h0 = T0(), T1 && h1 = T1(), T2 && h2 = T2(), T3 && h3 = T3(),			\
					 T4 && h4 = T4(), T5 && h5 = T5(), T6 && h6 = T6(), T7 && h7 = T7())			\
	{																								\
		func(QHttpHandlerRef(h0), QHttpHandlerRef(h1), QHttpHandlerRef(h2), QHttpHandlerRef(h3),	\
			QHttpHandlerRef(h4), QHttpHandlerRef(h5), QHttpHandlerRef(h6), QHttpHandlerRef(h7));	\
	}
#else
#define LAMBDA_IMPLICT_CONVERTION_WORKAROUND(func)
#endif

#define DECLARE_METHOD_METHOD(method)															\
	void method(QHttpHandlerRef h0 = QHttpHandlerRef(), QHttpHandlerRef h1 = QHttpHandlerRef(),	\
			 QHttpHandlerRef h2 = QHttpHandlerRef(), QHttpHandlerRef h3 = QHttpHandlerRef(),	\
			 QHttpHandlerRef h4 = QHttpHandlerRef(), QHttpHandlerRef h5 = QHttpHandlerRef(),	\
			 QHttpHandlerRef h6 = QHttpHandlerRef(), QHttpHandlerRef h7 = QHttpHandlerRef()) {	\
		this->use(new QHttpHandler_MethodFilter(#method), h0, h1, h2, h3, h4, h5, h6, h7);		\
	}																							\
	LAMBDA_IMPLICT_CONVERTION_WORKAROUND(method)


class Q_SERVER_EXPORT QHttpRouter : public QObject, QHttpHandler
{
	Q_OBJECT
public:
	QHttpRouter(QObject * parent = 0);

public:
	void use(QHttpHandlerRef h0 = QHttpHandlerRef(), QHttpHandlerRef h1 = QHttpHandlerRef(),
			 QHttpHandlerRef h2 = QHttpHandlerRef(), QHttpHandlerRef h3 = QHttpHandlerRef(), 
			 QHttpHandlerRef h4 = QHttpHandlerRef(), QHttpHandlerRef h5 = QHttpHandlerRef(),
			 QHttpHandlerRef h6 = QHttpHandlerRef(), QHttpHandlerRef h7 = QHttpHandlerRef(),
			 QHttpHandlerRef h8 = QHttpHandlerRef());
	LAMBDA_IMPLICT_CONVERTION_WORKAROUND(use);

	DECLARE_METHOD_METHOD(get);
public:
	virtual void invoke(QHttpContext & ctx);

private:
	QHttpHandlerSetChain m_hsc;
};

QT_END_NAMESPACE

#endif
