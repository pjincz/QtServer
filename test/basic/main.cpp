#include <QtCore/QtCore>
#include <QtNetwork/QtNetwork>
#include <QtServer>

int main(int argc, char * argv[])
{
	QCoreApplication app(argc, argv);

	QHttpService s;

	s.get("/", [](QHttpRequest & req, QHttpResponse & res) {
		res.body = "this is main page";
	});
	s.get("/hello", [](QHttpRequest & req, QHttpResponse & res) {
		res.body = "hello boy";
	});
	
	s.listen(3000);

	return app.exec();
}
