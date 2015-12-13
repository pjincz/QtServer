#ifndef QHTTPRESPONSE_H
#define QHTTPRESPONSE_H

#include "qtserverglobal.h"
#include "qhttpheaders.h"
#include <QtCore/QString>

QT_BEGIN_NAMESPACE

class QIODevice;

class QHttpResponseBody
{
public:
	enum {
		Empty,
		Buffer,
		File
	};

public:
	int type;
	QByteArray buffer;
	QString path;

public:
	inline QHttpResponseBody & operator= (const char * str) {
		type = Buffer;
		buffer = str;
		return *this;
	}
	inline QHttpResponseBody & operator= (const QByteArray & rhs) {
		type = Buffer;
		buffer = rhs;
		return *this;
	}
	inline QHttpResponseBody & setFilePath(const QString & path) {
		type = File;
		this->path = path;
		return *this;
	}
};

class Q_SERVER_EXPORT QHttpResponse
{
public:
	QHttpResponse();
	void serialize(QIODevice * d);

public:
	QHttpHeaders headers;
	QHttpResponseBody body;
	int status;
};

QT_END_NAMESPACE

#endif
