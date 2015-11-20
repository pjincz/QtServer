#ifndef QHTTPRESPONSE_H
#define QHTTPRESPONSE_H

#include "qtserverglobal.h"
#include "qhttpheaders.h"
#include <QtCore/QString>

QT_BEGIN_NAMESPACE

class Q_SERVER_EXPORT QHttpResponse
{
public:
	QHttpResponse();
	void reset();
	QByteArray serialize();

public:
	QHttpHeaders headers;
	QByteArray body;
	int status;
};

QT_END_NAMESPACE

#endif
