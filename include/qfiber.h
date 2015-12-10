#ifndef QFIBER_H
#define QFIBER_H

#include "qtserverglobal.h"
#include <QtCore/QObject>
#include <QtCore/QVariant>

QT_BEGIN_NAMESPACE

class QFiberPrivate;

typedef void (*FIBER_ENTRY)(QVariant);

class Q_SERVER_EXPORT QFiber : public QObject
{
	Q_OBJECT
public:
	QFiber(FIBER_ENTRY entry, const QVariant & arg = QVariant());
	~QFiber();

	void run();

protected:
	void resume();

public:
	static void wait(QObject * obj, const char * signal);

signals:
	void done();

protected slots:
	void wait_done();

private:
	QFiberPrivate *d;
};

QT_END_NAMESPACE

#endif
