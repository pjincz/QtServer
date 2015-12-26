#include <qhttpservice.h>
#include <QtDebug>

QT_BEGIN_NAMESPACE

QHttpContext::QHttpContext()
	: req(0), res(0), service(0), parent(0), chain(0), i(0), j(0)
	, url_shift(0), base_url_shift(0)
{
}

QHttpContext::QHttpContext(QHttpContext * parent)
	: req(parent->req), res(parent->res), service(parent->service), parent(parent), chain(0), i(0), j(0)
	, url_shift(parent->url_shift), base_url_shift(parent->url_shift)
{
}

class QHttpContextBackup
{
public:
	QHttpContextBackup(QHttpContext & ctx)
		: ctx(ctx)
	{
		url_shift = ctx.url_shift;
		req_params = ctx.req->params;
	}
	~QHttpContextBackup()
	{
		ctx.req->params = req_params;
		ctx.url_shift = url_shift;
	}

	QHttpContext & ctx;
	int url_shift;
	QHash<QString, QString> req_params;
};

void reset_handler_set_var(QHttpContext & ctx)
{
	ctx.url_shift = ctx.base_url_shift;
	ctx.req->params = QHash<QString, QString>();
}

void QHttpContext::next()
{
	QHttpContextBackup backup(*this);

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
			reset_handler_set_var(*this);
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
			reset_handler_set_var(*this);
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

