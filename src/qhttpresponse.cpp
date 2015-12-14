#include <qhttpservice.h>
#include <qhttprequest.h>
#include <qfiber.h>
#include "compile_info.h"
#include <QtCore/QtCore>

static const QString HTTP_SERVER = QString("QtServer/") + QS_VERSION + " (" + QS_OS + ")";

QT_BEGIN_NAMESPACE

QHttpResponse::QHttpResponse()
	: status(0)
{
	headers["Server"] = HTTP_SERVER;
	headers["Connection"] = "keep-alive";
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

static const char * status2str(int status)
{
	switch(status)
	{
		case 100: return "Continue";
		case 101: return "Switching Protocols";
		case 102: return "Processing";

		case 200: return "OK";
		case 201: return "Created";
		case 202: return "Accepted";
		case 203: return "Non-Authoritative Information";
		case 204: return "No Content";
		case 205: return "Reset Content";
		case 206: return "Partial Content";
		case 207: return "Multi-Status";
		case 208: return "Already Reported";
		case 226: return "IM Used";

		case 300: return "Multiple Choices";
		case 301: return "Moved Permanently";
		case 302: return "Found";
		case 303: return "See Other";
		case 304: return "Not Modified";
		case 305: return "Use Proxy";
		case 306: return "Switch Proxy";
		case 307: return "Temporary Redirect";
		case 308: return "Permanent Redirect";

		case 400: return "Bad Request";
		case 401: return "Unauthorized";
		case 402: return "Payment Required";
		case 403: return "Forbidden";
		case 404: return "Not Found";
		case 405: return "Method Not Allowed";
		case 406: return "Not Acceptable";
		case 407: return "Proxy Authentication Required";
		case 408: return "Request Timeout";
		case 409: return "Conflict";
		case 410: return "Gone";
		case 411: return "Length Required";
		case 412: return "Precondition Failed";
		case 413: return "Payload Too Large";
		case 414: return "URI Too Long";
		case 415: return "Unsupported Media Type";
		case 416: return "Range Not Satisfiable";
		case 417: return "Expectation Failed";
		case 418: return "I'm a teapot";
		case 419: return "Authentication Timeout";
		case 420: return "Enhance Your Calm";
		case 421: return "Misdirected Request";
		case 422: return "Unprocessable Entity";
		case 423: return "Locked";
		case 424: return "Failed Dependency";
		case 426: return "Upgrade Required";
		case 428: return "Precondition Required";
		case 429: return "Too Many Requests";
		case 431: return "Request Header Fields Too Large";
		case 440: return "Login Timeout";
		case 444: return "No Response";
		case 449: return "Retry With";
		case 450: return "Blocked by Windows Parental Controls";
		case 451: return "Unavailable For Legal Reasons";
		case 494: return "Request Header Too Large";
		case 495: return "Cert Error";
		case 496: return "No Cert";
		case 497: return "HTTP to HTTPS";
		case 498: return "Token expired/invalid";
		case 499: return "Client Closed Request";

		case 500: return "Internal Server Error";
		case 501: return "Not Implemented";
		case 502: return "Bad Gateway";
		case 503: return "Service Unavailable";
		case 504: return "Gateway Timeout";
		case 505: return "HTTP Version Not Supported";
		case 506: return "Variant Also Negotiates";
		case 507: return "Insufficient Storage";
		case 508: return "Loop Detected";
		case 509: return "Bandwidth Limit Exceeded";
		case 510: return "Not Extended";
		case 511: return "Network Authentication Required";
		case 520: return "Unknown Error";
		case 522: return "Origin Connection Time-out";
		case 598: return "Network read timeout error";
		case 599: return "Network connect timeout error";
	}
	return "Unknown";
}

static QByteArray serializeHeader(QHttpHeaders & h, int status, qint64 length)
{
	h["Content-Length"] = QString::number(length);

	QByteArray r = QByteArray("HTTP/1.1 ") + QByteArray::number(status) + 
				   " " + status2str(status) + "\r\n";
	for (QHttpHeaders::iterator it = h.begin(); it != h.end(); ++it) {
		r += capitalize(it.key()).toLatin1() + ": " + it.value() + "\r\n";
	}
	return r + "\r\n";
}

static void sendFile(QIODevice * d, QFile & f, qint64 start, qint64 end)
{
	static const int MAX_BUFFER = 1024 * 1024; // 1M

	if (start)
		f.seek(start);

	qint64 remain = end - start;

	while (remain) {
		int n = MAX_BUFFER - d->bytesToWrite();

		QByteArray a = f.read(n);
		d->write(a);
		remain -= a.length();

		if (remain) {
			QFiber::await(d, SIGNAL(bytesWritten(qint64)));
		}
	}
}

static QString etag(const QByteArray & a)
{
	return "\"" + QString::number(qHash(a)) + "\"";
}

static QString etag(QFile & f)
{
	QFileInfo fi(f);
	return "\"" + fi.lastModified().toString(Qt::ISODate) + "\"";
}

static void simpleResponse(QIODevice * d, int status, const char * body = NULL)
{
	QByteArray r = QByteArray("HTTP/1.1 ") + QByteArray::number(status) + 
				   " " + status2str(status) + "\r\n";
	r += "X-Powered-By: QtServer\r\n"
		 "Connection: keep-alive\r\n";
	if (body) {
		r += "Content-Type: text/plain; charset=utf-8\r\n";
		r += "Content-Length: " + QByteArray::number((int)strlen(body)) + "\r\n";
	}
	r += "\r\n";

	if (body) {
		r += body;
	}

	d->write(r);
}

void serialize_file(QIODevice * d, QHttpRequest * req, QHttpResponse * res)
{
	QFile file(res->body.path);
	if (!file.exists()) {
		qWarning() << "QHttpResponse: file not exists" << res->body.path;
		simpleResponse(d, 500, "500 Internal Server Error");
		return;
	}

	QString e = etag(file);
	if (req->headers["If-None-Match"] == e) {
		simpleResponse(d, 304);
		return;
	}
	res->headers["ETag"] = e;

	if (req->method == "HEAD") {
		d->write(serializeHeader(res->headers, res->status ? res->status : 200, 0));
		return;
	}

	qint64 total = file.size();
	qint64 start = 0;
	qint64 end = file.size();

	if (req->headers.contains("Range") && (res->status == 0 || res->status == 200)) {
		static const QRegExp x("(\\w*)=(\\d*)-(\\d*)");
		x.indexIn(req->headers["Range"]);
		if (x.cap(1) == "bytes") {
			if (!x.cap(2).isEmpty())
				start = x.cap(2).toLongLong();
			if (!x.cap(3).isEmpty())
				end = x.cap(3).toLongLong() + 1;

			if (end > total)
				end = total;

			if (end > start) {
				res->headers["Content-Range"] = QString().sprintf("bytes %lld-%lld/%lld", start, end - 1, total);
				res->status = 206;
			} else {
				start = 0;
				end = total;
			}
		}
	}

	file.open(QIODevice::ReadOnly);
	d->write(serializeHeader(res->headers, res->status ? res->status : 200, end - start));
	sendFile(d, file, start, end);
}

void QHttpResponse::serialize(QIODevice * d, QHttpRequest * req)
{
	if (body.type == QHttpResponseBody::Empty) {
		if (status == 0) {
			if (req->method == "HEAD") {
				simpleResponse(d, 404);
			} else {
				simpleResponse(d, 404, "404 Not Found");
			}
		} else {
			d->write(serializeHeader(headers, status, 0));
		}
	} else if (body.type == QHttpResponseBody::Buffer) {
		QString e = etag(body.buffer);
		if (req->headers["If-None-Match"] == e) {
			simpleResponse(d, 304);
			return;
		}
		headers["ETag"] = e;
		if (req->method == "HEAD") {
			d->write(serializeHeader(headers, status ? status : 200, 0));
			return;
		}
		d->write(serializeHeader(headers, status ? status : 200, body.buffer.length()));
		d->write(body.buffer);
	} else if (body.type == QHttpResponseBody::File) {
		serialize_file(d, req, this);
	} else {
		qWarning() << "QHttpResponse: unknown body type" << body.type;
		simpleResponse(d, 500, "500 Internal Server Error");
	}
}

QT_END_NAMESPACE

