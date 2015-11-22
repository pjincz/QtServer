#ifndef QPROMISE_H
#define QPROMISE_H

#include "qtserverglobal.h"
#include <QtCore/QObject>
#include <QtCore/QVariant>
#include "qfiber.h"

QT_BEGIN_NAMESPACE

class Q_SERVER_EXPORT QPromiseBase : public QObject
{
	Q_OBJECT
public:
	QPromiseBase();

public:
	void fullfill(const QVariant & v);
	void reject(const QVariant & v);

signals:
	void fullfilled(const QVariant & v);
	void rejected(const QVariant & v);
};

template<typename T>
class Q_SERVER_EXPORT QPromise : public QPromiseBase
{
public:
	inline void fullfill(const T & v)
	{
		QPromiseBase::fullfill(QVariant::fromValue<T>(v));
	}
	inline T yield()
	{
		QVariant v = QFiber::yield(this);
		return v.value<T>();
	}
};

QT_END_NAMESPACE

#endif
