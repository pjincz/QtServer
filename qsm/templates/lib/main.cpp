#include <QtServer>
#include "QSM_NAME.h"

void myfiber(QVariant /*x*/)
{
	qDebug() << hello::hello();
}

int main(int argc, char * argv[])
{
	QCoreApplication app(argc, argv);

	QFiber * fib = new QFiber(myfiber);
	QObject::connect(fib, SIGNAL(done()), &app, SLOT(quit()), Qt::QueuedConnection);
	fib->run();

	return app.exec();
}
