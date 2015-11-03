#ifndef QFIBER_H
#define QFIBER_H

#include "qtserverglobal.h"
#include <QtCore/QObject>
#include <QtCore/QVariant>

QT_BEGIN_NAMESPACE

class QPromiseBase;
template<typename T> class QPromise;

class QFiberPrivate;

typedef void (*FIBER_ENTRY)(QVariant);

class Q_CORE_EXPORT QFiber : public QObject
{
	Q_OBJECT
public:
	QFiber(FIBER_ENTRY entry, const QVariant & arg = QVariant());
	~QFiber();

private:
	void resume(const QVariant & passin, bool rejected);

public:
	static QVariant yield_base(QPromiseBase * promise);

	template<typename T>
	static inline T yield(QPromise<T> * promise) 
	{
		return yield_base((QPromiseBase*)promise).value<T>();
	}

private slots:
	void promise_fullfilled(const QVariant & var);
	void promise_rejected(const QVariant & var);

private:
	QFiberPrivate *d;
};

QT_END_NAMESPACE

#endif
