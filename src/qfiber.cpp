#include <qfiber.h>
#include <qpromise.h>
#include "3rdparty/libcoro/coro.h"

QT_BEGIN_NAMESPACE

class QFiberPrivate;

class QFiberGlobal
{
public:
	QFiberGlobal()
		: current(0), promise(NULL), rejected(false)
	{
		coro_create(&mainctx, NULL, NULL, NULL, 0);
	}

public:
	coro_context mainctx;

	QFiberPrivate * current;
	QPromiseBase * promise;
	QVariant passin;
	bool rejected;
};

class QFiberPrivate
{
public:
	coro_context ctx;
	struct coro_stack stack;

	FIBER_ENTRY entry;
	QVariant arg;
	QFiber * q;
};

static QFiberGlobal * global() 
{
	static QFiberGlobal global;
	return &global;
}

void coro_body(void * arg)
{
	QFiberPrivate * d = (QFiberPrivate *)arg;
	// initialized
	coro_transfer(&d->ctx, &global()->mainctx);
	
	// working
	try
	{
		d->entry(d->arg);
	}
	catch(...)
	{
	}

	// done
	coro_transfer(&d->ctx, &global()->mainctx);
}

QFiber::QFiber(FIBER_ENTRY entry, const QVariant & arg)
{
	d = new QFiberPrivate;
	d->entry = NULL;
	d->arg = arg;
	coro_stack_alloc(&d->stack, 0);
	coro_create(&d->ctx, coro_body, NULL, d->stack.sptr, d->stack.ssze);

	// transfer to initialize
	coro_transfer(&global()->mainctx, &d->ctx);

	resume(QVariant(), false);
}

QFiber::~QFiber()
{
	coro_destroy(&d->ctx);
	coro_stack_free(&d->stack);
}

void QFiber::resume(const QVariant & passin, bool rejected)
{
	QFiberGlobal * g = global();

	g->promise = NULL;
	g->passin = passin;
	g->rejected = rejected;
	g->current = d;
	coro_transfer(&g->mainctx, &d->ctx);
	g->current = NULL;

	if (g->promise)
	{
		connect(g->promise, SIGNAL(fullfilled(const QVariant&)), this, SLOT(promise_fullfilled(const QVariant&)));
		connect(g->promise, SIGNAL(rejected(const QVariant&)), this, SLOT(promise_rejected(const QVariant&)));
	}
	else
	{
		deleteLater();
	}
}

void QFiber::promise_fullfilled(const QVariant & var)
{
	Q_ASSERT(qobject_cast<QPromiseBase*>(sender()));
	sender()->deleteLater();
	resume(var, false);
}

void QFiber::promise_rejected(const QVariant & var)
{
	Q_ASSERT(qobject_cast<QPromiseBase*>(sender()));
	sender()->deleteLater();
	resume(var, true);
}

QT_END_NAMESPACE
