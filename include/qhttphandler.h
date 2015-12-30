#ifndef QHTTPHANDLER_H
#define QHTTPHANDLER_H

#include "qtserverglobal.h"
#include "qhttpcontext.h"
#include <QSharedPointer>
#include <functional>

#ifndef QS_GET_MACRO3
#define QS_GET_MACRO3(_1, _2, _3, NAME, ...) NAME
#endif

#ifndef QS_LAMBDA
#define QS_LAMBDA1(ctx) [=](QHttpContext & ctx)
#define QS_LAMBDA2(req, res) [=](QHttpRequest & req, QHttpResponse & res)
#define QS_LAMBDA3(req, res, ctx) [=](QHttpRequest & req, QHttpResponse & res, QHttpContext & ctx)
#define QS_LAMBDA(...) QS_GET_MACRO3(__VA_ARGS__, QS_LAMBDA3, QS_LAMBDA2, QS_LAMBDA1)(__VA_ARGS__)
#endif

QT_BEGIN_NAMESPACE

class QHttpRequest;
class QHttpResponse;

typedef void LAMBDA_HANDLER_SIGN1(QHttpRequest & req, QHttpResponse & res);
typedef void LAMBDA_HANDLER_SIGN2(QHttpRequest & req, QHttpResponse & res, QHttpContext & ctx);
typedef void LAMBDA_HANDLER_SIGN3(QHttpContext & ctx);
typedef int LAMBDA_HANDLER_SIGN4(QHttpRequest & req, QHttpResponse & res);
typedef int LAMBDA_HANDLER_SIGN5(QHttpRequest & req, QHttpResponse & res, QHttpContext & ctx);
typedef int LAMBDA_HANDLER_SIGN6(QHttpContext & ctx);

class Q_SERVER_EXPORT QHttpHandler
{
public:
	enum {
		NOOP = 0,
		CONTINUE = 1,
		SKIP = 2,
	};
public:
	virtual ~QHttpHandler();

public:
	virtual int invoke(QHttpContext & ctx);
};

template<typename T, typename _Sign>
class QHttpHandlerLambdaWrap;

template<typename T>
class QHttpHandlerLambdaWrap<T, LAMBDA_HANDLER_SIGN1> : public QHttpHandler {
public:
	QHttpHandlerLambdaWrap(const T & rhs) : impl(rhs) {}
	virtual int invoke(QHttpContext & ctx) {
		this->impl(*ctx.req, *ctx.res);
		return 0;
	}
private:
	T impl;
};
template<typename T>
class QHttpHandlerLambdaWrap<T, LAMBDA_HANDLER_SIGN2> : public QHttpHandler {
public:
	QHttpHandlerLambdaWrap(const T & rhs) : impl(rhs) {}
	virtual int invoke(QHttpContext & ctx) {
		this->impl(*ctx.req, *ctx.res, ctx);
		return 0;
	}
private:
	T impl;
};
template<typename T>
class QHttpHandlerLambdaWrap<T, LAMBDA_HANDLER_SIGN3> : public QHttpHandler {
public:
	QHttpHandlerLambdaWrap(const T & rhs) : impl(rhs) {}
	virtual int invoke(QHttpContext & ctx) {
		this->impl(ctx);
		return 0;
	}
private:
	T impl;
};
template<typename T>
class QHttpHandlerLambdaWrap<T, LAMBDA_HANDLER_SIGN4> : public QHttpHandler {
public:
	QHttpHandlerLambdaWrap(const T & rhs) : impl(rhs) {}
	virtual int invoke(QHttpContext & ctx) {
		return this->impl(*ctx.req, *ctx.res);
	}
private:
	T impl;
};
template<typename T>
class QHttpHandlerLambdaWrap<T, LAMBDA_HANDLER_SIGN5> : public QHttpHandler {
public:
	QHttpHandlerLambdaWrap(const T & rhs) : impl(rhs) {}
	virtual int invoke(QHttpContext & ctx) {
		return this->impl(*ctx.req, *ctx.res, ctx);
	}
private:
	T impl;
};
template<typename T>
class QHttpHandlerLambdaWrap<T, LAMBDA_HANDLER_SIGN6> : public QHttpHandler {
public:
	QHttpHandlerLambdaWrap(const T & rhs) : impl(rhs) {}
	virtual int invoke(QHttpContext & ctx) {
		return this->impl(ctx);
	}
private:
	T impl;
};

class Q_SERVER_EXPORT QHttpHandlerRef : public QSharedPointer<QHttpHandler>
{
public:
	QHttpHandlerRef();
	QHttpHandlerRef(QHttpHandler * h);
	QHttpHandlerRef(const char * path);

	// TODO: use a stupid way to walk around int(...) can be converted to void(...)
	// Need a better way, this really ugly...
	template<typename T>
	QHttpHandlerRef(const T & rhs, 
			typename std::enable_if<std::is_convertible<T, std::function<LAMBDA_HANDLER_SIGN1>>::value>::type * dummy = 0,
			typename std::enable_if<!std::is_convertible<T, std::function<LAMBDA_HANDLER_SIGN4>>::value>::type * dummy2 = 0) {
		reset(new QHttpHandlerLambdaWrap<T, LAMBDA_HANDLER_SIGN1>(rhs));
	}
	template<typename T>
	QHttpHandlerRef(const T & rhs, 
			typename std::enable_if<std::is_convertible<T, std::function<LAMBDA_HANDLER_SIGN2>>::value>::type * dummy = 0,
			typename std::enable_if<!std::is_convertible<T, std::function<LAMBDA_HANDLER_SIGN5>>::value>::type * dummy2 = 0) {
		reset(new QHttpHandlerLambdaWrap<T, LAMBDA_HANDLER_SIGN2>(rhs));
	}
	template<typename T>
	QHttpHandlerRef(const T & rhs, 
			typename std::enable_if<std::is_convertible<T, std::function<LAMBDA_HANDLER_SIGN3>>::value>::type * dummy = 0,
			typename std::enable_if<!std::is_convertible<T, std::function<LAMBDA_HANDLER_SIGN6>>::value>::type * dummy2 = 0) {
		reset(new QHttpHandlerLambdaWrap<T, LAMBDA_HANDLER_SIGN3>(rhs));
	}
	template<typename T>
	QHttpHandlerRef(const T & rhs, 
			typename std::enable_if<std::is_convertible<T, std::function<LAMBDA_HANDLER_SIGN4>>::value>::type * dummy = 0) {
		reset(new QHttpHandlerLambdaWrap<T, LAMBDA_HANDLER_SIGN4>(rhs));
	}
	template<typename T>
	QHttpHandlerRef(const T & rhs, 
			typename std::enable_if<std::is_convertible<T, std::function<LAMBDA_HANDLER_SIGN5>>::value>::type * dummy = 0) {
		reset(new QHttpHandlerLambdaWrap<T, LAMBDA_HANDLER_SIGN5>(rhs));
	}
	template<typename T>
	QHttpHandlerRef(const T & rhs, 
			typename std::enable_if<std::is_convertible<T, std::function<LAMBDA_HANDLER_SIGN6>>::value>::type * dummy = 0) {
		reset(new QHttpHandlerLambdaWrap<T, LAMBDA_HANDLER_SIGN6>(rhs));
	}

	template<typename T>
	QHttpHandlerRef(const T & rhs,
			typename std::enable_if<std::is_base_of<QHttpHandler, T>::value>::type * dummy = 0) {
		static_assert(std::is_copy_constructible<T>::value, "Cannot move Object from Stack to heap, Object is not copyable");
		reset(new T(rhs));
	}

public:
	inline int invoke(QHttpContext & ctx)
	{
		return data()->invoke(ctx);
	}
};

class QHttpHandlerSet : public QVector<QHttpHandlerRef>
{
public:
	QString method;
};
typedef QVector<QHttpHandlerSet> QHttpHandlerSetChain;

QT_END_NAMESPACE

#endif
