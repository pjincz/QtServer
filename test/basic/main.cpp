#include <QtCore/QtCore>
#include <QtNetwork/QtNetwork>
#include <QtServer>

int main(int argc, char * argv[])
{
	QCoreApplication app(argc, argv);

	QHttpService s;
	
	s.listen(3000);

	return app.exec();
}
