#ifndef QFIBER_H
#define QFIBER_H

#include "qtserverglobal.h"
#include <QtCore/QObject>
#include <QtCore/QVariant>

QT_BEGIN_NAMESPACE

class QPromiseBase;

class QFiberPrivate;

typedef void (*FIBER_ENTRY)(QVariant);

class Q_SERVER_EXPORT QFiber : public QObject
{
	Q_OBJECT
public:
	QFiber(FIBER_ENTRY entry, const QVariant & arg = QVariant());
	~QFiber();

	void run();

private:
	void resume(const QVariant & passin, bool rejected);

public:
	static QVariant yield(QPromiseBase * promise);

private slots:
	void promise_fullfilled(const QVariant & var);
	void promise_rejected(const QVariant & var);

signals:
	void done();

private:
	QFiberPrivate *d;
};

QT_END_NAMESPACE

#endif
