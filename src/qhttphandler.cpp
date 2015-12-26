#include <qhttphandler.h>
#include <qhttpcontext.h>
#include <qhttprequest.h>

QT_BEGIN_NAMESPACE

////////////////////////////////////////////////////////////////////////////////
// QHttpHandler

QHttpHandler::~QHttpHandler()
{
}

int QHttpHandler::invoke(QHttpContext & ctx)
{
	return CONTINUE;
}

////////////////////////////////////////////////////////////////////////////////
// handlers

class QHttpUrlFilter : public QHttpHandler
{
public:
	QHttpUrlFilter(const char * exp)
		: m_exp(exp)
	{
	}
	int invoke(QHttpContext & ctx)
	{
		return m_exp == ctx.req->url.path() ? CONTINUE : SKIP;
	}

private:
	QString m_exp;
};

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
	this->reset(new QHttpUrlFilter(exp));
}

QT_END_NAMESPACE

