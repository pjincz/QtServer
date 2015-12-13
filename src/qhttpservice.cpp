#include <QTcpServer>
#include <QTcpSocket>
#include <qhttpservice.h>
#include <qfiber.h>

QT_BEGIN_NAMESPACE

class QHttpConnection : public QObject
{
	Q_OBJECT

public:
	QHttpConnection(QHttpService * service, QTcpSocket * socket)
		: QObject(service), m_service(service), m_socket(socket)
	{
		socket->setParent(this);

		QFiber * f = new QFiber(fiber_entry, QVariant::fromValue((void*)this));
		connect(m_socket, SIGNAL(disconnected()), f, SLOT(terminal()));
		connect(f, SIGNAL(finished()), this, SLOT(deleteLater()));

		f->run();
	}

protected:
	static void fiber_entry(QVariant v)
	{
		QHttpConnection * c = ((QHttpConnection*)v.value<void *>());
		c->run();
	}
	void fetch()
	{
		if (!m_socket->bytesAvailable()) {
			QFiber::await(m_socket, SIGNAL(readyRead()));
		}
		m_cache += m_socket->readAll();
	}
	QByteArray takeHeader()
	{
		int x;
		int l;
		for(;;) {
			x = m_cache.indexOf("\r\n\r\n", 0);
			l = 4;
			if (x == -1)
			{
				x = m_cache.indexOf("\n\n", 0);
				l = 2;
			}
			else
			{
				int y = m_cache.indexOf("\n\n", 0);
				if (y != -1 && y < x)
				{
					x = y;
					l = 2;
				}
			}
			if (x != -1)
				break;
			fetch();
		}
		QByteArray header = m_cache.left(x);
		m_cache = m_cache.mid(x + l);
		return header;
	}
	QByteArray takeBody(int len)
	{
		while (m_cache.length() < len) {
			fetch();
		}
		QByteArray body = m_cache.left(len);
		m_cache = m_cache.mid(len);
		return body;
	}
	void run()
	{
		for (;;) {
			QHttpRequest req;
			req.parse(takeHeader());

			if (req.headers.contains("Content-Length")) {
				req.body = takeBody(req.headers["Content-Length"].toInt());
			}

			QHttpResponse res;
			QHttpContext ctx;
			ctx.req = &req;
			ctx.res = &res;
			ctx.service = m_service;

			m_service->invoke(ctx);

			m_socket->write(res.serialize());
			if (req.protocol == "HTTP/1.0") {
				return;
			}
		}
	}

private:
	QHttpService * m_service;
	QTcpSocket * m_socket;
	QByteArray m_cache;
};

QHttpService::QHttpService(QObject * parent)
	: QHttpRouter(parent)
{
}

void QHttpService::listen(int port)
{
	QTcpServer * x = new QTcpServer(this);
	connect(x, SIGNAL(newConnection()), this, SLOT(onNewConnection()));
	x->listen(QHostAddress::Any, 3000);
}

void QHttpService::onNewConnection()
{
	QTcpServer * s = qobject_cast<QTcpServer*>(sender());
	if (!s)
		return;
	
	while (QTcpSocket * socket = s->nextPendingConnection())
	{
		new QHttpConnection(this, socket);
	}
}

QT_END_NAMESPACE

#include "qhttpservice.moc"
