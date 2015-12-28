#include <qhttphandler.h>
#include <qhttpcontext.h>
#include <qhttprequest.h>

QT_BEGIN_NAMESPACE

////////////////////////////////////////////////////////////////////////////////
// QHttpHandler

QHttpHandler::~QHttpHandler()
{
}

int QHttpHandler::invoke(QHttpContext & ctx)
{
	return CONTINUE;
}

////////////////////////////////////////////////////////////////////////////////
// handlers

class QHttpUrlFilter : public QHttpHandler
{
public:
	QHttpUrlFilter(const char * urlexp_)
	{
		QString urlexp(urlexp_);
		if (urlexp[0] == '/')
			urlexp = urlexp.mid(1);
		QStringList parts = urlexp.split('/');

		QString regexp = "^";
		for (int i = 0; i < parts.length(); ++i) {
			QString part = parts[i];
			regexp += "/";
			if (part[0] == ':') {
				regexp += "([^/]*)";
				m_capNames.push_back(part.mid(1));
			} else if (part == "*") {
				regexp += "[^/]*";
			} else if (part == "**") {
				regexp += ".*";
			} else {
				regexp += QRegExp::escape(part);
			}
		}
		m_exp = QRegExp(regexp);
	}
	bool isInUseStatement(QHttpContext & ctx)
	{
		if (ctx.chain && ctx.i < ctx.chain->length()) {
			return ctx.chain->at(ctx.i).method == "use";
		}
		return false;
	}
	virtual int invoke(QHttpContext & ctx)
	{
		QString path = ctx.req->url.path();
		bool matched = false;

		if (isInUseStatement(ctx)) {
			// only required to match start part
			int ir = m_exp.indexIn(path, ctx.url_shift, QRegExp::CaretAtOffset);
			if (ir != -1) {
				if (path.length() - ctx.url_shift == m_exp.matchedLength() 
						|| path[ctx.url_shift + m_exp.matchedLength()] == '/') {
					matched = true;
				}
			}
		} else {
			// have to match all
			if (m_exp.exactMatch(path.mid(ctx.url_shift)))
			{
				matched = true;
			}
		}
		
		if (matched) {
			QHash<QString, QString> params;
			for (int i = 0; i < m_capNames.length(); ++i) {
				params[m_capNames[i]] = m_exp.cap(i + 1);
			}
			ctx.req->params = params;
			ctx.url_shift += m_exp.matchedLength();
			return CONTINUE;
		} else {
			return SKIP;
		}
	}

private:
	QRegExp m_exp;
	QStringList m_capNames;
};

////////////////////////////////////////////////////////////////////////////////
// QHttpHandlerRef

QHttpHandlerRef::QHttpHandlerRef()
{
}

QHttpHandlerRef::QHttpHandlerRef(QHttpHandler * h)
{
	this->reset(h);
}

QHttpHandlerRef::QHttpHandlerRef(const char * exp)
{
	this->reset(new QHttpUrlFilter(exp));
}

QT_END_NAMESPACE

