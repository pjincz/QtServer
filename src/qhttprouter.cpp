#include <qhttprouter.h>
#include <qhttpservice.h>

QT_BEGIN_NAMESPACE

QHttpRouter::QHttpRouter(QObject * parent)
	: QObject(parent)
{
}

void QHttpRouter::use(QHttpHandlerRef h0, QHttpHandlerRef h1,
					  QHttpHandlerRef h2, QHttpHandlerRef h3, 
					  QHttpHandlerRef h4, QHttpHandlerRef h5,
					  QHttpHandlerRef h6, QHttpHandlerRef h7,
					  QHttpHandlerRef h8)
{
	QHttpHandlerSet s;
	if (!h0.isNull()) s.push_back(h0);
	if (!h1.isNull()) s.push_back(h1);
	if (!h2.isNull()) s.push_back(h2);
	if (!h3.isNull()) s.push_back(h3);
	if (!h4.isNull()) s.push_back(h4);
	if (!h5.isNull()) s.push_back(h5);
	if (!h6.isNull()) s.push_back(h6);
	if (!h7.isNull()) s.push_back(h7);
	if (!h8.isNull()) s.push_back(h8);

	m_hsc.push_back(s);
}

void QHttpRouter::invoke(QHttpContext & ctx)
{
	QHttpContext ctx2;
	ctx2.req = ctx.req;
	ctx2.res = ctx.res;
	if (QHttpService * s = qobject_cast<QHttpService *>(this))
	{
		ctx2.service = s;
	}
	else
	{
		ctx2.service = ctx.service;
	}
	ctx2.chain = &m_hsc;

	ctx2.next();
}

QT_END_NAMESPACE

