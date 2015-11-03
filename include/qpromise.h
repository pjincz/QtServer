#ifndef Q_PROMISE
#define Q_PROMISE

#include "qtserverglobal.h"
#include <QtCore/QObject>
#include <QtCore/QVariant>

QT_BEGIN_NAMESPACE

class QPromiseBase : public QObject
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
class QPromise : public QPromiseBase
{
public:
	inline void fullfill(const T & v)
	{
		QPromiseBase::fullfill(QVariant::fromValue<T>(v));
	}
};

QT_END_NAMESPACE

#endif
