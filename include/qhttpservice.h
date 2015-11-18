#ifndef QHTTPSERVICE_H
#define QHTTPSERVICE_H

#include <QtCore/QObject>
#include "qhttprequest.h"
#include "qhttpresponse.h"
#include "qhttpcontext.h"

QT_BEGIN_NAMESPACE

class QHttpService : public QObject
{
	Q_OBJECT
public:
	QHttpService(QObject * parent = 0);

	// TODO
	void invoke(QHttpRequest & req, QHttpResponse &res, QHttpContext & ctx);

	void listen(int port);

protected slots:
	void onNewConnection();
};

QT_END_NAMESPACE

#endif
