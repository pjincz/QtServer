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

static QString capitalize(const QString & x)
{
	QString r = x;
	bool cap = true;
	for (int i = 0; i < r.length(); ++i)
	{
		if (r[i].isLetterOrNumber())
		{
			if (cap)
				r[i] = r[i].toUpper();
			cap = false;
		}
		else
		{
			cap = true;
		}
	}
	return r;
}

QByteArray QHttpResponse::serialize()
{
	headers["Content-Length"] = QString::number(body.length());
	QByteArray r = QByteArray("HTTP/1.1 ") + QByteArray::number(status) + QByteArray(" OK\r\n");
	for (QHttpHeaders::iterator it = headers.begin(); it != headers.end(); ++it)
	{
		r += capitalize(it.key()).toLatin1() + ": " + it.value() + "\r\n";
	}
	r += "\r\n";
	return r + body;
}

QT_END_NAMESPACE

