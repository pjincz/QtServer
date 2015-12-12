#ifndef QFIBER_H
#define QFIBER_H

#include "qtserverglobal.h"
#include <QtCore/QObject>
#include <QtCore/QVariant>

QT_BEGIN_NAMESPACE

class QFiberPrivate;

typedef void (*FIBER_ENTRY)(QVariant);

class QAWaitAgent : public QObject
{
	// this class hack Q_OBJECT
	// so, do not add Q_OBJECT
public:
	enum {
		TIMEOUT = -1,
		LOGIC_ERROR = -2
	};
public:
	QAWaitAgent();

public:
	void setTimeout(int timeout);
	void wait(QObject * obj, const char * signal);
	QVariantList result();

protected:
	virtual bool event(QEvent * e);
	virtual int qt_metacall(QMetaObject::Call call, int methodId, void **a);
	void achieved(void **a);

private:
	QList<QPair<QObject *, int> > m_list;
	class QFiber * m_fiber;
	QVariantList m_result;
};

class Q_SERVER_EXPORT QFiber : public QObject
{
	Q_OBJECT
	friend class QAWaitAgent;
public:
	QFiber(FIBER_ENTRY entry, const QVariant & arg = QVariant());
	~QFiber();

	void run();

signals:
	void finished();

public:
	static void await(QAWaitAgent & agent);

	static inline QVariantList await(
			QObject * o1, const char * s1, 
			int timeout = 3000) {
		QAWaitAgent a;
		a.wait(o1, s1);
		a.setTimeout(timeout);
		await(a);
		return a.result();
	}
	static inline QVariantList await(
			QObject * o1, const char * s1, 
			QObject * o2, const char * s2, 
			int timeout = 3000) {
		QAWaitAgent a;
		a.wait(o1, s1);
		a.wait(o2, s2);
		a.setTimeout(timeout);
		await(a);
		return a.result();
	}

protected slots:
	void terminal();

protected:
	void resume();

private:
	QFiberPrivate *d;
};

QT_END_NAMESPACE

#endif
