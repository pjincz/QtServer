#include <qhttpservice.h>

QT_BEGIN_NAMESPACE

QHttpContext::QHttpContext()
	: req(0), res(0), service(0), chain(0), i(0), j(0)
{
}

void QHttpContext::next()
{
	if (!chain)
		return;

	while (i < chain->length())
	{
		bool skip = false;
		while (j < chain->at(i).length())
		{
			QHttpHandlerRef h = chain->at(i)[j];
			int f = h.filter(*this);
			if (f == QHttpHandler::NOT_A_FILTER)
			{
				break;
			}
			else if (f == QHttpHandler::FILTER_THROUGH)
			{
				++j;
				continue;
			}
			else
			{
				skip = true;
				break;
			}
		}
		if (j >= chain->at(i).length() || skip)
		{
			++i;
			j = 0;
			continue;
		}
		QHttpHandlerRef h = chain->at(i)[j];
		++j;
		h.invoke(*this);
	}
}

void QHttpContext::skip()
{
	++i;
	j = 0;
	QHttpContext::next();
}

QT_END_NAMESPACE

