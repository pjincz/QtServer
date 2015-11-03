#include <qpromise.h>

QT_BEGIN_NAMESPACE

QPromiseBase::QPromiseBase()
{
}

void QPromiseBase::fullfill(const QVariant & v)
{
	emit fullfilled(v);
}

void QPromiseBase::reject(const QVariant & v)
{
	emit reject(v);
}

QT_END_NAMESPACE
