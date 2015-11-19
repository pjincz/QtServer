#include <qhttphandler.h>

QT_BEGIN_NAMESPACE

QHttpHandlerRef::QHttpHandlerRef()
{
}

QHttpHandlerRef::QHttpHandlerRef(const char * exp)
{
	this->reset(new QHttpHandler_UrlFilter(exp));
}

QHttpHandlerRef::QHttpHandlerRef(FUNCTION_HANDLER_SIGN1 func)
{
	this->reset(new QHttpHandler_Method1(func));
}

QT_END_NAMESPACE

