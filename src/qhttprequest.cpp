#include <qhttpservice.h>
#include <QStringList>
#include <QUrlQuery>

QT_BEGIN_NAMESPACE

QHttpRequest::QHttpRequest()
{
}

void QHttpRequest::parse(const QByteArray & a)
{
	QString str = QString::fromLatin1(a);
	QStringList lines;

	int pos_start = 0;
	for (int i = 0; i < str.length(); ++i)
	{
		if (str[i] == '\n')
		{
			if (i > 0 && str[i - 1] == '\r')
			{
				lines.append(str.mid(pos_start, i - pos_start - 1));
			}
			else
			{
				lines.append(str.mid(pos_start, i - pos_start));
			}
			pos_start = i + 1;
		}
	}
	if (pos_start != str.length())
	{
		lines.append(str.mid(pos_start));
	}

	QStringList first_line = lines[0].split(' ');
	this->method = first_line[0].toUpper();
	this->url = first_line[1];
	this->protocol = first_line[2];

	typedef QPair<QString, QString> QPair_String_String;
	foreach (QPair_String_String x, QUrlQuery(this->url).queryItems()) {
		this->query.insert(x.first, x.second);
	}

	for (int i = 1; i < lines.length(); ++i)
	{
		int colon_pos = lines[i].indexOf(':');
		if (colon_pos != -1)
		{
			this->headers.insert(lines[i].left(colon_pos), lines[i].mid(colon_pos + 1).trimmed());
		}
	}

	this->body = QByteArray();
}

QT_END_NAMESPACE

