#ifndef QHTTPREQUEST_H
#define QHTTPREQUEST_H

#include "qtserverglobal.h"
#include "qhttpheaders.h"
#include <QtCore/QString>
#include <QtCore/QUrl>

QT_BEGIN_NAMESPACE

class Q_SERVER_EXPORT QHttpRequest
{
public:
	QHttpRequest();
	void parse(const QByteArray &);

public:
	QHttpHeaders headers;
	QString method;
	QUrl url;
	QString protocol;
	QByteArray body;
};

QT_END_NAMESPACE

#endif
