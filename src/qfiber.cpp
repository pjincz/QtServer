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

////////////////////////////////////////////////////////////////////////////////
// QAWaitAgent

QAWaitAgent::QAWaitAgent()
{
	m_fiber = global()->current->q;
}

void QAWaitAgent::setTimeout(int timeout)
{
	if (timeout >= 0)
		startTimer(timeout);
}

void QAWaitAgent::wait(QObject * obj, const char * signal)
{
	static const int memberOffset = QObject::staticMetaObject.methodCount();

	const QByteArray ba = QMetaObject::normalizedSignature(signal + 1);
	const QMetaObject * const mo = obj->metaObject();
	const int sigIndex = mo->indexOfMethod(ba.constData());
	if (sigIndex < 0) {
		qWarning() << "QAWaitAgent: Not a valid signal:" << signal;
		// in order to keep index of result, put a placeholder here
		m_list.append(qMakePair(obj, sigIndex));
		return;
	}

	QMetaObject::connect(obj, sigIndex, this, memberOffset, Qt::DirectConnection, 0);

	m_list.append(qMakePair(obj, sigIndex));
}

QVariantList QAWaitAgent::result()
{
	return m_result;
}

bool QAWaitAgent::event(QEvent * e)
{
	if (e->type() == QEvent::Timer) {
		m_result << TIMEOUT;
		m_fiber->resume();
		return true;
	}

	return QObject::event(e);
}

int QAWaitAgent::qt_metacall(QMetaObject::Call call, int methodId, void **a)
{
	methodId = QObject::qt_metacall(call, methodId, a);
	if (methodId < 0)
		return methodId;

	if (call == QMetaObject::InvokeMetaMethod) {
		if (methodId == 0) {
			achieved(a);
		}
		--methodId;
	}
	return methodId;
}

void QAWaitAgent::achieved(void **a)
{
	QObject * o = sender();
	int s = senderSignalIndex();

	int i;
	for (i = 0; i < m_list.length(); ++i) {
		if (m_list[i].first == o && m_list[i].second == s) {
			break;
		}
	}
	if (i < m_list.length()) {
		QMetaMethod m = m_list[i].first->metaObject()->method(m_list[i].second);
		//qDebug() << m.methodSignature();

		m_result.reserve(m.parameterCount() + 1);
		m_result << i;
		for (int j = 0; j < m.parameterCount(); ++j) {
			int tp = m.parameterType(j);
            if (tp == QMetaType::UnknownType) {
                void *argv[] = { &tp, &j };
                QMetaObject::metacall(o,
                                      QMetaObject::RegisterMethodArgumentMetaType,
                                      i, argv);
                if (tp == -1)
                    tp = QMetaType::UnknownType;
            }
            if (tp == QMetaType::UnknownType) {
                qWarning("QAWaitAgent: Don't know how to handle '%s', use qRegisterMetaType to register it.",
                         m.parameterNames().at(j).constData());
            }
			if (tp == QMetaType::QVariant)
				m_result << *reinterpret_cast<QVariant *>(a[j + 1]);
			else
				m_result << QVariant(tp, a[j + 1]);
		}
	} else {
		// LOGIC ERROR
		qWarning() << "QAWaitAgent: achieved by unknown signal" << o << s;
		m_result << LOGIC_ERROR;
	}
	m_fiber->resume();
}

////////////////////////////////////////////////////////////////////////////////
// QFiber

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
	QFiberGlobal * g = global();

	d->reset();
	g->current = d;
	coro_transfer(&g->mainctx, &d->ctx);
	g->current = NULL;

	//qDebug() << "resume" << d->out_goon;

	if (!d->out_goon)
	{
		emit finished();
		deleteLater();
	}
}

void QFiber::await(QAWaitAgent & agent)
{
	//qDebug() << "await";

	QFiberGlobal * g = global();
	g->current->out_goon = true;
	coro_transfer(&g->current->ctx, &g->mainctx);

	//qDebug() << agent.result();
}

void QFiber::terminal()
{
	// TODO
}

QT_END_NAMESPACE
