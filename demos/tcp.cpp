#include <QtCore/QtCore>
#include <QtNetwork/QtNetwork>
#include <stdio.h>

class QHttpService;
class QHttpConnection;

class QHttpRequest
{
public:
	QHttpRequest()
	{
	}
	void parse(const QByteArray & a)
	{
		headers.clear();

		QString str = QString::fromLatin1(a);
		QStringList lines;

		int pos_start = 0;
		for (int i = 0; i < str.length(); ++i)
		{
			if (str[i] == '\n')
			{
				if (i > 0 && str[i - 1] == '\r')
				{
					lines.append(str.mid(pos_start, i - pos_start - 1));
				}
				else
				{
					lines.append(str.mid(pos_start, i - pos_start));
				}
				pos_start = i + 1;
			}
		}
		if (pos_start != str.length())
		{
			lines.append(str.mid(pos_start));
		}

		QStringList first_line = lines[0].split(' ');
		this->method = first_line[0];
		this->originalUrl = first_line[1];
		this->protocol = first_line[2];

		for (int i = 1; i < lines.length(); ++i)
		{
			int colon_pos = lines[i].indexOf(':');
			if (colon_pos != -1)
			{
				this->headers.insert(lines[i].left(colon_pos).toLower(), lines[i].mid(colon_pos + 1).trimmed());
			}
		}
	}
public:
	QHash<QString, QString> headers;
	QString method;
	QUrl originalUrl;
	QString protocol;
};

class QHttpResponse
{
};

class QHttpContext
{
public:
	QHttpContext()
	{
	}
	virtual void next()
	{
	}
	virtual void skip()
	{
	}
};

class QHttpService : public QObject
{
	Q_OBJECT
public:
	QHttpService(QObject * parent = 0)
		: QObject(parent)
	{
	}

	// TODO
	void invoke(QHttpRequest & req, QHttpResponse & res, QHttpContext & ctx)
	{
		qDebug() << req.method;
		qDebug() << req.originalUrl;
		qDebug() << req.protocol;
		qDebug() << req.headers;
	}

	void listen(int port)
	{
		QTcpServer * x = new QTcpServer(this);
		connect(x, SIGNAL(newConnection()), this, SLOT(onNewConnection()));
		x->listen(QHostAddress::Any, 3000);
	}

protected slots:
	void onNewConnection();
};

class QHttpConnection : public QObject
{
	Q_OBJECT
public:
	enum STATUS {
		PENDING_HEADER,
		PENDING_BODY,
		PENDING_INVOKE,
		PENDING_RESPONSE,
		PENDING_DESTORY
	};
public:
	QHttpConnection(QHttpService * service, QTcpSocket * socket)
		: QObject(service), m_service(service), m_socket(socket), m_status(PENDING_HEADER)
	{
		connect(socket, SIGNAL(readyRead()), this, SLOT(onData()));
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
				m_status = PENDING_INVOKE;
				//m_status = m_req.headers["Content-Length"] ? PENDING_BODY : PENDING_RESPONSE;
			}
			if (m_status == PENDING_BODY)
			{
				// TODO
			}
			if (m_status == PENDING_INVOKE)
			{
				m_res = QHttpResponse();
				m_status = PENDING_RESPONSE;
				m_service->invoke(m_req, m_res, m_ctx);
			}
			if (m_status == PENDING_RESPONSE)
			{
				return;
			}
		}
	}

protected slots:
	void onData()
	{
		m_cache += m_socket->readAll();
		goon();
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

int main(int argc, char * argv[])
{
	QCoreApplication app(argc, argv);

	QHttpService s;

	s.listen(3000);

	return app.exec();
}

#include "tcp.moc"
