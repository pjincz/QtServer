#include <qhttpservice.h>

QT_BEGIN_NAMESPACE

QHttpResponse::QHttpResponse()
{
	reset();
}

void QHttpResponse::reset()
{
	headers.clear();
	headers["Content-Type"] = "text/plain";
	headers["Connection"] = "Keep-Alive";
	body = QByteArray();
	status = 200;
}

QByteArray QHttpResponse::serialize()
{
	//qDebug() << "serialize()";
	return "HTTP/1.1 200 OK\r\nX-Powered-By: Express\r\nContent-Type: text/html; charset=utf-8\r\nContent-Length: 11\r\nDate: Tue, 17 Nov 2015 15:37:36 GMT\r\nConnection: close\r\n\r\n" + body;
	
	headers["Content-Length"] = QString::number(body.length());
	QByteArray r = QByteArray("HTTP/1.1 ") + QByteArray::number(status) + QByteArray(" OK\r\n");
	for (QHttpHeaders::iterator it = headers.begin(); it != headers.end(); ++it)
	{
		r += it.key().toLatin1() + ": " + it.value() + "\r\n";
	}
	r += "\r\n";
	return r + body;
}

QT_END_NAMESPACE

