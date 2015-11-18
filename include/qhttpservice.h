#ifndef QHTTPSERVICE_H
#define QHTTPSERVICE_H

#include "qhttprouter.h"

QT_BEGIN_NAMESPACE

class QHttpService : public QHttpRouter
{
	Q_OBJECT
public:
	QHttpService(QObject * parent = 0);

	void listen(int port);

protected slots:
	void onNewConnection();
};

QT_END_NAMESPACE

#endif
