#include <QTcpServer>
#include <QTcpSocket>
#include <qhttpservice.h>
#include <qfiber.h>

QT_BEGIN_NAMESPACE

class QHttpConnection : public QObject
{
	Q_OBJECT
public:
	enum STATUS {
		PENDING_HEADER,
		PENDING_BODY,
		PENDING_INVOKE,
		PENDING_RESPONSE,
		PENDING_DESTROY
	};
public:
	QHttpConnection(QHttpService * service, QTcpSocket * socket)
		: QObject(service), m_service(service), m_socket(socket), m_status(PENDING_HEADER)
	{
		socket->setParent(this);
		connect(socket, SIGNAL(readyRead()), this, SLOT(onData()));
		connect(socket, SIGNAL(disconnected()), this, SLOT(onDisconnected()));
		m_ctx.req = &m_req;
		m_ctx.res = &m_res;
		m_ctx.service = service;
	}

protected:
	void goon()
	{
		for (;;)
		{
			if (m_status == PENDING_HEADER)
			{
				int x = m_cache.indexOf("\r\n\r\n", 0);
				int l = 4;
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
				if (x == -1)
					return;
				QByteArray header = m_cache.left(x);
				m_cache = m_cache.mid(x + l);
				m_req.parse(header);
				//m_status = PENDING_INVOKE;
				m_status = m_req.headers.contains("Content-Length") ? PENDING_BODY : PENDING_INVOKE;
			}
			if (m_status == PENDING_BODY)
			{
				int l = m_req.headers["Content-Length"].toInt();
				if (m_cache.length() >= l)
				{
					m_req.body = m_cache.left(l);
					m_cache = m_cache.mid(l);
					m_status = PENDING_INVOKE;
				}
				else
				{
					return;
				}
			}
			if (m_status == PENDING_INVOKE)
			{
				m_res.reset();
				m_status = PENDING_RESPONSE;
				launchInFiber();
			}
			if (m_status == PENDING_RESPONSE)
			{
				return;
			}
		}
	}
	void launchInFiber()
	{
		QFiber * f = new QFiber(fiberEntry, QVariant::fromValue((void*)&m_ctx));
		connect(f, SIGNAL(done()), this, SLOT(onFiberDone()));
		f->run();
	}
	static void fiberEntry(QVariant v)
	{
		QHttpContext * ctx = (QHttpContext *)v.value<void *>();
		ctx->service->invoke(*ctx);
	}

protected slots:
	void onData()
	{
		m_cache += m_socket->readAll();
		goon();
	}
	void onDisconnected()
	{
		if (m_status == PENDING_RESPONSE)
		{
			m_status = PENDING_DESTROY;
		}
		else
		{
			deleteLater();
		}
	}
	void onFiberDone()
	{
		if (m_status == PENDING_DESTROY)
		{
			deleteLater();
		}
		else
		{
			m_socket->write(m_res.serialize());
			if (m_req.protocol == "HTTP/1.0")
			{
				m_socket->close();
			}
			m_status = PENDING_HEADER;
			goon();
		}
	}

private:
	QHttpService * m_service;
	QTcpSocket * m_socket;
	STATUS m_status;
	QByteArray m_cache;
	QHttpRequest m_req;
	QHttpResponse m_res;
	QHttpContext m_ctx;
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
