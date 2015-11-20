#ifndef QHTTPREQUEST_H
#define QHTTPREQUEST_H

#include "qtserverglobal.h"
#include "qhttpheaders.h"
#include <QtCore/QString>
#include <QtCore/QUrl>

QT_BEGIN_NAMESPACE

class QHttpRequest
{
public:
	QHttpRequest();
	void parse(const QByteArray &);

public:
	QHttpHeaders headers;
	QString method;
	QUrl url;
	QString protocol;
};

QT_END_NAMESPACE

#endif
