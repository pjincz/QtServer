#include <qhttpservice.h>

QT_BEGIN_NAMESPACE

QHttpContext::QHttpContext()
	: req(0), res(0)
{
}

void QHttpContext::next()
{
}

void QHttpContext::skip()
{
}

QT_END_NAMESPACE

