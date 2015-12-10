#include <qfiber.h>
#include "3rdparty/libcoro/coro.h"
#include <QtCore>

QT_BEGIN_NAMESPACE

class QFiberPrivate;

class QFiberGlobal
{
public:
	QFiberGlobal()
		: current(0)
	{
		coro_create(&mainctx, NULL, NULL, NULL, 0);
	}

public:
	coro_context mainctx;

	QFiberPrivate * current;
};

class QFiberPrivate
{
public:
	coro_context ctx;
	struct coro_stack stack;

	FIBER_ENTRY entry;
	QVariant arg;
	QFiber * q;

	// fiber -> main
	bool out_goon;

	void reset()
	{
		out_goon = false;
	}
};

static QFiberGlobal * global() 
{
	static QFiberGlobal global;
	return &global;
}

void coro_body(void * arg)
{
	QFiberPrivate * d = (QFiberPrivate *)arg;

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
	d->entry = entry;
	d->arg = arg;
	d->q = this;
	d->reset();
	coro_stack_alloc(&d->stack, 0);
	coro_create(&d->ctx, coro_body, d, d->stack.sptr, d->stack.ssze);
}

void QFiber::run()
{
	resume();
}

QFiber::~QFiber()
{
	coro_destroy(&d->ctx);
	coro_stack_free(&d->stack);

	// qDebug() << 111;
	delete d;
	// qDebug() << 222;
}

void QFiber::resume()
{
	// qDebug() << "resume" << d->out_goon;
	
	QFiberGlobal * g = global();

	d->reset();
	g->current = d;
	coro_transfer(&g->mainctx, &d->ctx);
	g->current = NULL;

	if (!d->out_goon)
	{
		emit done();
		deleteLater();
	}
}

void QFiber::wait(QObject * obj, const char * signal)
{
	QFiberGlobal * g = global();
	connect(obj, signal, g->current->q, SLOT(wait_done()));

	g->current->out_goon = true;
	coro_transfer(&g->current->ctx, &g->mainctx);
}

void QFiber::wait_done()
{
	disconnect(sender(), 0, 0, 0);

	resume();
}

QT_END_NAMESPACE
