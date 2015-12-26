#include <qhttpservice.h>
#include <QtDebug>

QT_BEGIN_NAMESPACE

QHttpContext::QHttpContext()
	: req(0), res(0), service(0), parent(0), chain(0), i(0), j(0)
{
}

QHttpContext::QHttpContext(QHttpContext * parent)
	: req(parent->req), res(parent->res), service(parent->service), parent(parent), chain(0), i(0), j(0)
{
}

void QHttpContext::next()
{
	if (!chain) {
		if (parent)
			parent->next();
		return;
	}

	for (;;) {
		// over last set, invoke parent, and set i -> n + 1
		if (i == chain->length()) {
			++i;
			if (parent)
				parent->next();
			return;
		}
		// over again, this is a bug in logic
		if (i > chain->length()) {
			qWarning() << "next invoked more than once";
			return;
		}
		if (j >= chain->at(i).length()) {
			j = 0;
			++i;
			continue;
		}
		// deal normal case
		QHttpHandlerRef h = chain->at(i)[j];
		++j;
		int si = i, sj = j;
		int ir = h.invoke(*this);
		if (ir == QHttpHandler::NOOP) {
			// break chain, there are 2 cases
			// 1 deal do not need continue
			// 2 handler call ctx.next() or ctx.skip() in body
			break;
		} else if (ir == QHttpHandler::CONTINUE) {
			// handler require to continue chain,
			// in this case, handler should not invoke ctx.next() or ctx.skip()
			if (si != i || sj != j) {
				qWarning() << h << "request to continue after invoke next() or skip(), conflicted";
				break;
			}
			continue;
		} else if (ir == QHttpHandler::SKIP) {
			// handler require to skip one chain set,
			// in this case, handler should not invoke ctx.next() or ctx.skip()
			if (si != i || sj != j) {
				qWarning() << h << "request to skip after invoke next() or skip(), conflicted";
				break;
			}
			++i;
			j = 0;
			continue;
		} else {
			qWarning() << h << "bad return value!";
			break;
		}
	}
}

void QHttpContext::skip()
{
	++i;
	j = 0;
	QHttpContext::next();
}

QT_END_NAMESPACE

