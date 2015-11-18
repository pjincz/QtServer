#include <QtCore/QtCore>
#include "qfiber.h"
#include "qpromise.h"

class SleepPromise : public QPromise<int>
{
public:
	SleepPromise(int ms)
	{
		startTimer(ms);
	}
	void timerEvent(QTimerEvent *event)
	{
		killTimer(event->timerId());
		fullfill(1);
	}
};

QPromise<int> * sleep(int n)
{
	return new SleepPromise(n);
}

void myfiber(QVariant v)
{
	printf("hello\n");
	int x = QFiber::yield(sleep(1000));
	printf("%d\n", x);
	printf("world\n");
}

int main(int argc, char * argv[])
{
	QCoreApplication app(argc, argv);

	printf("begin...\n");
	new QFiber(myfiber);

	app.exec();
}
