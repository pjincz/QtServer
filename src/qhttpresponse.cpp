#include <qhttpservice.h>
#include <qfiber.h>
#include <QtCore/QtCore>

QT_BEGIN_NAMESPACE

QHttpResponse::QHttpResponse()
	: status(0)
{
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

static QByteArray serializeHeader(QHttpHeaders & h, int status, qint64 length)
{
	h["Content-Length"] = QString::number(length);

	QByteArray r = QByteArray("HTTP/1.1 ") + QByteArray::number(status) + QByteArray(" OK\r\n");
	for (QHttpHeaders::iterator it = h.begin(); it != h.end(); ++it) {
		r += capitalize(it.key()).toLatin1() + ": " + it.value() + "\r\n";
	}
	return r + "\r\n";
}

static void sendFile(QIODevice * d, QFile & f)
{
	static const int MAX_BUFFER = 1024 * 1024; // 1M

	while (f.bytesAvailable()) {
		int n = MAX_BUFFER - d->bytesToWrite();
		QByteArray a = f.read(n);
		d->write(a);

		if (f.bytesAvailable()) {
			QFiber::await(d, SIGNAL(bytesWritten(qint64)));
		}
	}
}

void QHttpResponse::serialize(QIODevice * d)
{
	if (body.type == QHttpResponseBody::Empty) {
		if (status == 0) {
			QByteArray b = "404 Not Found";
			d->write(serializeHeader(headers, 404, b.length()));
			d->write(b);
		} else {
			d->write(serializeHeader(headers, status, 0));
		}
	} else if (body.type == QHttpResponseBody::Buffer) {
		d->write(serializeHeader(headers, status || 200, body.buffer.length()));
		d->write(body.buffer);
	} else if (body.type == QHttpResponseBody::File) {
		QFile file(body.path);
		if (file.exists()) {
			file.open(QIODevice::ReadOnly);
			d->write(serializeHeader(headers, status || 200, file.size()));
			sendFile(d, file);
		} else {
			QByteArray b = "404 Not Found";
			d->write(serializeHeader(headers, 404, b.length()));
			d->write(b);
		}
	} else {
		qWarning() << "QHttpResponse: unknown body type" << body.type;
		QByteArray b = "500 Internal Server Error";
		d->write(serializeHeader(headers, 500, b.length()));
		d->write(b);
	}
}

QT_END_NAMESPACE

