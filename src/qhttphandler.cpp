#include <qhttphandler.h>
#include <qhttpcontext.h>
#include <qhttprequest.h>

QT_BEGIN_NAMESPACE

////////////////////////////////////////////////////////////////////////////////
// QHttpHandler

QHttpHandler::~QHttpHandler()
{
}

int QHttpHandler::filter(QHttpContext &)
{
	return NOT_A_FILTER;
}

void QHttpHandler::invoke(QHttpContext & ctx)
{
	ctx.next();
}

////////////////////////////////////////////////////////////////////////////////
// QHttpHandlerRef

QHttpHandlerRef::QHttpHandlerRef()
{
}

QHttpHandlerRef::QHttpHandlerRef(QHttpHandler * h)
{
	this->reset(h);
}

QHttpHandlerRef::QHttpHandlerRef(const char * exp)
{
	this->reset(new QHttpHandler_UrlFilter(exp));
}

QHttpHandlerRef::QHttpHandlerRef(FUNCTION_HANDLER_SIGN1 func)
{
	this->reset(new QHttpHandler_Method1(func));
}

////////////////////////////////////////////////////////////////////////////////
// Handlers

QHttpHandler_MethodFilter::QHttpHandler_MethodFilter(const char * method)
{
	m_method = QString::fromLatin1(method).toUpper();
}

int QHttpHandler_MethodFilter::filter(QHttpContext & ctx)
{
	if (ctx.req->method == "HEAD" && m_method == "GET")
		return FILTER_THROUGH;
	return m_method == ctx.req->method ? FILTER_THROUGH : FILTER_FILTERED;
}


QHttpHandler_UrlFilter::QHttpHandler_UrlFilter(const char * exp)
	: m_urlexp(exp)
{
}

int QHttpHandler_UrlFilter::filter(QHttpContext & ctx)
{
	// TODO
	return ctx.req->url.path() == m_urlexp ? FILTER_THROUGH : FILTER_FILTERED;
}


QHttpHandler_Method1::QHttpHandler_Method1(FUNCTION_HANDLER_SIGN1 func)
	: m_func(func)
{
}

void QHttpHandler_Method1::invoke(QHttpContext & ctx)
{
	m_func(*ctx.req, *ctx.res);
}

QT_END_NAMESPACE

