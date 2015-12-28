#include <qhttprouter.h>
#include <qhttpservice.h>

QT_BEGIN_NAMESPACE

class QHttpMethodFilter : public QHttpHandler
{
public:
	QHttpMethodFilter(const char * method) 
	{
		this->method = QString(method).toUpper();
	}

	virtual int invoke(QHttpContext & ctx)
	{
		if (ctx.req->method == method)
			return QHttpHandler::CONTINUE;
		if (ctx.req->method == "HEAD" && method == "GET")
			return QHttpHandler::CONTINUE;
		return QHttpHandler::SKIP;
	}

private:
	QString method;
};

QHttpRouter::QHttpRouter()
{
}

void QHttpRouter::handle(const char * method,
						QHttpHandlerRef h0, QHttpHandlerRef h1,
						QHttpHandlerRef h2, QHttpHandlerRef h3, 
						QHttpHandlerRef h4, QHttpHandlerRef h5,
						QHttpHandlerRef h6, QHttpHandlerRef h7)
{
	QHttpHandlerSet s;
	s.method = method;

	if (strcmp(method, "use") != 0 && strcmp(method, "all") != 0) {
		s.push_back(QHttpHandlerRef(new QHttpMethodFilter(method)));
	}

	if (!h0.isNull()) s.push_back(h0);
	if (!h1.isNull()) s.push_back(h1);
	if (!h2.isNull()) s.push_back(h2);
	if (!h3.isNull()) s.push_back(h3);
	if (!h4.isNull()) s.push_back(h4);
	if (!h5.isNull()) s.push_back(h5);
	if (!h6.isNull()) s.push_back(h6);
	if (!h7.isNull()) s.push_back(h7);

	m_hsc.push_back(s);
}

int QHttpRouter::invoke(QHttpContext & ctx)
{
	QHttpContext ctx2(&ctx);
	if (QHttpService * s = dynamic_cast<QHttpService *>(this))
	{
		ctx2.service = s;
	}
	ctx2.chain = &m_hsc;

	ctx2.next();

	return 0;
}

QT_END_NAMESPACE

