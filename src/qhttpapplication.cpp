#include <stdio.h>
#include <unistd.h>
#include <qhttpapplication.h>

QT_BEGIN_NAMESPACE

static void myMessageOutput(QtMsgType type, const QMessageLogContext &context, const QString &msg)
{
	QByteArray localMsg = msg.toLocal8Bit();

	const char * color = "";
	const char * colorend = "";
	const char * prefix = "";

	if (isatty(fileno(stderr))) {
		switch(type) {
		case QtDebugMsg:
			color = "\033[0;49;37m";
			break;
		case QtWarningMsg:
			color = "\033[0;49;33m";
			break;
		case QtCriticalMsg:
			color = "\033[0;49;31m";
			break;
		case QtFatalMsg:
			color = "\033[0;41;39m";
			break;
		}
		colorend = "\033[0m";
	} else {
		switch(type) {
		case QtDebugMsg:
			prefix = "debug: ";
			break;
		case QtWarningMsg:
			prefix = "warning: ";
			break;
		case QtCriticalMsg:
			prefix = "critical: ";
			break;
		case QtFatalMsg:
			prefix = "fatal: ";
			break;
		}
	}

	fprintf(stderr, "%s%s%s%s\n", color, prefix, localMsg.constData(), colorend);

	if (type == QtFatalMsg) {
		fprintf(stderr, "%s%s(%s:%u, %s)%s\n", color, prefix, context.file, context.line, context.function, colorend);
		abort();
	}
}

QHttpApplication::QHttpApplication(int & argc, char ** argv)
	: coreApp(argc, argv)
{
	qInstallMessageHandler(myMessageOutput);
}

QT_END_NAMESPACE

